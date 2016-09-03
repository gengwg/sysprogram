#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  // creat
#include <errno.h>  // errno
#include <limits.h> // SSIZE_MAX
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // truncate


/* new architectures can implement creat() as a library all
   invoking open() syscall.
int creat (const char *name, int mode)
{
    return open (name, O_WRONLY |O_CREAT | O_TRUNC, mode);
}
*/

int main ()
{
    int fd;

    // fd = open ("/home/gengwg/test", O_WRONLY | O_TRUNC);

    // this is effective to below.
    const char *myfile = "./test2";
    fd = creat (myfile, 0644);

    // if the file does not exist, it is created with the permission 0644.
    // if it does exist, it is truncaated to zero length
    // fd = open ("/home/gengwg/test3", O_WRONLY |O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        printf("%s\n", "file open error");
        return -1;
    }

    printf("file descriptor number: %i\n", fd);



    fd = open ("./test", O_RDONLY);
    if (fd == -1)
    {
        printf("%s\n", "file open error");
        return -1;
    }
    printf("file descriptor number: %i\n", fd);

    unsigned long word;
    ssize_t nr;
    

    // read 8 bytes from 'fd' into 'word'
    // 4 bytes on 32-bit, 8 bytes on 64-bit
    //
    // ssize_t read (int fd, void *buf, size_t len);
    // Reads up to len bytes into the memory pointed by buf
    // from the current file offset of the file referenced by fd.
    // on success, returns number of bytes written into buf;
    // on error, returns -1 and sets errno.
    // A call to read with len of zeror results in the call returning 
    // immediately wiht a return value of 0.
    nr = read (fd, &word, sizeof (unsigned long)); 
    if (nr == -1)
    {
        printf("%s\n", "file read error");
        return -1;

    }

    printf("read %i bytes\n", nr);

    /** Readng all the bytes **/

    unsigned long buf;
    ssize_t ret;
    size_t len = 134;
    // size_t type is used for stroing values usd to measure size in bytes
    // ssize_t is a singed version of size_t (negative values connote errors)
    // backing C type: unsinged int and int on 32 bit systems

    while (len != 0 && (ret = read (fd, &buf, len)) != 0) {
        if (len > SSIZE_MAX)
            len = SSIZE_MAX;

        if (ret == -1) {
            if (errno == EINTR)
                continue;
            perror("read");
            break;
        }

        len -= ret;
        buf += ret;
        printf("read %i bytes\n", ret);
    }
    
    printf("read %i bytes\n", ret);

    /** Non-blocking Reads **/

    /* if O_NONBLOCK was given to open() and there is no data to read,
       the read() call will return -1 and set errno to EAGAIN instead 
       of blocking.
       non-blocking i/o:
       call returns immediately, indicating no data is available.
       allows applications to perform i/o on multiple files,
       w/o blocking, thus missing data available in other files.


    char buf[BUFSIZ];
    ssize_t nr;

    start:
    nr = read (fd, buf, BUFSIZ);
    if (nr == -1) {
        if (errno == EINTR)
            goto start;
        if (errno == EAGAIN)
            // resubmit later 
        else
            // error
    }

     */

    /**Writing with write()**/

    /*
       ssize_t write (int fd, const void *buf, size_t count);
       writes up to count bytes starting at buf to the current position of the file 
       referenced by the file descriptor fd.
       on success, the num of bytes written is returned, and the file position is updated.
       on error, -1 is returned and errno is set appropriately.
       return 0 simply implies that zero bytes were written.
     */

    const char *buf2 = "My ship is solid!";
    len = strlen(buf2);

    const char *myfile2 = "./test4";
    fd = creat (myfile2, 0644);
    printf("file descriptor number: %i\n", fd);

    if (fd == -1)
        perror("file open error");    

    /*
       this is not quite right. callers need to check possible
       occurrence of a partial write.
     */
    nr = write (fd, buf2, len);
    if (nr == -1)
        perror("file write error");    

    /*
       for regular files no need to performa writes in a loop.
       for sockets, a loop is required to guarantee that 
       you really write out all of the requested bytes.
     */

    while (len != 0 && (ret = write (fd, buf2, len)) != 0) {
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            perror("write");
            break;
        }

        len -= ret;
        buf += ret;
    }

    /*
       fsync() ensures that all dirty data associated with the file 
       mapped by the file discriptor fd are written back to disk.
       it writes both data and metadata, such as creation timestamps 
       and other attributes contained in the inode.

       in the presense of hard disks with write cache, it is not possible
       for fsync() to know whether the data is physically on the disk.
       the hard drive can report that the data was written, but the data
       may in fact reside in the drive's write cache.

       fsync() always results at least 2 i/o operations:
       1. to write back the modified data
       2. to update the inode's modification timestamp
       the inode and the file's data may not be adjacent on disk,
       and thus require an expensive seek operation.
       most concerns over proper transaction ordering do not include
       metadata that is not essential to properly access the file,
       such as the modification timestamp, fdatasync() is an easy
       performance win.

       fdatasync() only flushes data nad metadata required to properly
       access the file in the future.

       neither guarantees any updated direcotry entries containing the
       file are synchronized to disk, rendering the file unreachable.
       to ensure, fsync() must also be called on a file descriptor
       opened against the file's directory.
    */

    ret = fsync (fd);
    if (ret == -1)
        perror("file sync error");

    // try fdatasync() if fsync() returns EINVAL
    if (fsync (fd) == -1) {
        if (errno == EINVAL) {
            if (fdatasync (fd) == -1)
                perror("fdatasync");
        } else
            perror("fsync");
    }

    // umpas the open fd and disassociates the file from the process
    // kernel is free to reuse it as the return value to a subsequent
    // open() or creat() call.
    if (close (fd) == -1)
        perror ("close");
    // linux does wait untill all buffers -- both data and metadata --
    // are commited to disk
    sync();

    /**
     * writeback max buffer age:
     centiseconds: 1/100 of a second
     $ cat /proc/sys/vm/dirty_expire_centisecs 
     3000
     */

    const char *myfile3 = "./test5";
    // O_SYNC flag indicates all I/O on the file should be synchronized
    // Read are always synchronized.
    fd = open (myfile3, O_WRONLY | O_SYNC);
    printf("file descriptor number: %i\n", fd);
    if (fd == -1) {
        perror ("open");
    }

    off_t ret2; 

    // set the file position of fd to 124
    ret2 = lseek(fd, (off_t) 124, SEEK_SET);
    printf("current position: %i\n", ret2);
    if (ret2 == (off_t) -1)
        perror("SEEK_SET"); 
    /*
       seeking past eof does nothing.
       a read request to the newly created file position
       will return EOF.
       a write request made to this position, new space will be created
       between the old length of the file and the new length, and 
       it will be padded with zeros.

       this zero padding is called a hole. holes do not occupy any
       physical disk space. the total size of all files on a filesystem
       can add up to more than the physical size of the disk.
       files with holes --> sparse files.

     */

    // set the file position of fd to end of the file
    ret2 = lseek(fd, 0, SEEK_END);
    printf("current position: %i\n", ret2);
    if (ret2 == (off_t) -1) {
        perror("SEEK_SET"); 
        return -1;
    }

    // find the current file position of fd
    // this will return the same as above end of file position
    int pos;
    pos = lseek(fd, 0, SEEK_CUR);
    printf("current position: %i\n", pos);
    if (pos == (off_t) -1) {
        perror("SEEK_SET"); 
        return -1;
    }

    close (fd);

    /*
       max file position is size of off_t --> C long type
       on linux, is the word size.
       internally, kernel stores the offsets in the C long long type.
     */
    
    /* Positional Reads and Writes

       #define _XOPEN_SOURCE 500
       #include <unistd.h>
       
       ssize_t pread (int fd, void *buf, size_t count, off_t pos);
       reads up to count bytes into buf from the fd at file position pos.

       ssize_t pwrite (int fd, const void *buf, size_t count, off_t pos);
       writes up to count bytes from buf to the fd at file position pos.

       when done they don't updated the file position.
     */

    /* Truncating Files

       2 system calls truncate the given file to the length given by len.
       data previously between len and old len is discarded and no longer
       accessible via a read request.

       #include <unistd.h>
       #include <sys/types.h>

       // operates on file descriptor given by fd, which must be open.
       int ftruncate (int fd, off_t len);

       // operates on file name given by path, which must be writable.
       int truncate (const char *path, off_t len);

       neither updates current file position.
       
       Original file:
       $ cat pirate.txt 
       Edward Teach was a notorious English pirate.
       He was nicknamed Blackbead.

       After running code below:
       $ cat pirate.txt 
       Edward Teach was a notorious English pirate.

     */

    int ret3;

    ret3 = truncate ("./pirate.txt", 45);
    if (ret3 == -1) {
        perror("truncate"); 
        return -1;
    }

    /* an application issues a read() system call. 
       
       the C lib provides definitions of the system call that are converted 
       to the appropriate trap statements at compile time. Once a user-space
       process is trapped into the kernel, passed through the system call handler,
       and hanled to the read() system call, the kernel figures out what object
       backs the given file descriptor. kenerl then invokes the read function 
       associated with the backing object. for filesystem, this function is part of
       the file system code. the functin then does its thing, physically reading the data
       from the filesystem, and returns the data to the user-space read() call, which
       then returns to the system call handler, which copies the data back to user space,
       where the read() system call returns and the process continues to execute
     */

    /* Page Writeback

       when a process issues a write request, the data is copied into a buffer,
       and the buffer is marked _dirty_, denoting that the in-momory copy is 
       newer than the on-disk copy. the write request simply returns. if another 
       write requst is made to the same chunk of file, the buffer is updated
       with the new data. write requests elsewhere in the same file generates
       new buffers.

       buffers are represented in the kernel by the buffer_head data structure.
       this data structure tracks various metadata associated with the buffer,
       such as whether the buffer is clean or dirty. also contains a pointer to 
       the actual data. this data resides in the page cache. ==> buffer subsystem
       and page cache are unified.
     */
    printf("program ends\n");
    return 0;
}

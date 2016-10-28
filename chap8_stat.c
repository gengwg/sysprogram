/* use stat() to retrieve the size of a file 

   stat() stores information in stat structure:

    struct stat {
        dev_t st_dev;       // ID of device containing file 
        ino_t st_ino;       // inode number
        mode_t st_mode;     // permissions
        nlink_t st_nlink;   // number of hard links
        uid_t st_uid;       // user Id of owner
        gid_t st_gid;       // group id of owner
        dev_t st_rdev;      // device ID (if special file)
        off_t st_size;      // total size in bytes
        blksize_t st_blksize;   // block size for filesystem I/O
        blkcnt_t st_blocks; // number of blocks allocated
        time_t st_atime;    // last access time
        time_t st_mtime;    // last modification time
        time_t st_ctime;    // last status change time
    }
   
   st_blksize is the preferred block size for efficient file/io. 
   is the optimal block size for user-buffered I/O.

   st_ctime is not creation time! it's not preserved on linux or other unix systems!

 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

/* returns a postive integer if 'fd' resides on a physical device,
   0 if resides on a nonphysical or virtual device (e.g. NFS mount),
   -1 on error.
 */
int is_on_physical_device (int fd)
{
    struct stat sb;
    int ret;

    ret = fstat (fd, &sb);
    if (ret) {
        perror ("fstat");
        return -1;
    }

    return sb.st_dev;
    //return gnu_dev_major (sb.st_dev);
}

int main (int argc, char *argv[])
{
    struct stat sb;
    int ret;

    if (argc < 2) {
        fprintf (stderr,
                "usage: %s <file>\n", argv[0]);
        return 1;
    }
    // int stat (const char *path, struct stat *buff);
    ret = stat (argv[1], &sb);
    if (ret) {
        perror ("stat");
        return 1;
    }

    printf ("%s is %ld bytes\n",
            argv[1], sb.st_size);


    printf ("File type: ");
    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
        printf("block device mode\n");
        break;
    case S_IFCHR:
        printf("character device mode\n");
        break;
    case S_IFIFO:
        printf("FIFO\n");
        break;
    case S_IFDIR:
        printf("directory\n");
        break;
    case S_IFLNK:
        printf("symbolic link\n");
        break;
    case S_IFREG:
        printf("regular file\n");
        break;
    case S_IFSOCK:
        printf("socket\n");
        break;
    default:
        printf("unknown\n");
        break;
    }

    printf("%d\n", is_on_physical_device(27));
    return 0;

}
/*
   $ ./a.out chap8_stat.c
   chap8_stat.c is 1528 bytes
 */

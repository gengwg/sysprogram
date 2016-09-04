#include <stdio.h>
#include <limits.h>

#define _GNU_SOURCE
//#define LINE_MAX 80

/* Read aribitrary strings

   reads the n-1 bytes from stream into s, then append the \0 character.
   */
char *fgetn(char *s, int n, FILE *stream) {
    int c;

    while (--n > 0 && (c = fgetc(stream)) != EOF)
        *s++ = c;
    *s = '\0';

    return s;
}

int main(void) {
    FILE *stream;

    stream = fopen("/tmp/tt", "r");
    if (!stream) {
        perror("stream");
        return -1;
    }

    /*
       #include <stdio.h>
       char * fgets (char *str, int size, FILE *stream);

       fgets() function reads up to one less than size bytes from stream
       and stores the result in str. A null character (\0) is stored in 
       the buffer after the last byte read in.
       Reading stops after an EOF or newline character is reached.
       if a new line is read, the \n is stored in str.

       on success, str is returned; on failure, NULL is returned.
     */

    // read 1 line first
    char buf[LINE_MAX];
    if (!fgets (buf, LINE_MAX, stream)) {
        perror("fgets");
        return -1;
    }

    printf("line read: %s\n", buf);

    // then read next 6 characters (including \n)
    if (!fgetn (buf, 7, stream)) {
        perror("fputs");
        return -1;
    }

    printf("str read: %s\n", buf);

    fclose(stream);

    /* Writing a string of characters

       int fputs (const char *str, FILE *stream);

       open file for writing in append mode,
       writes the given string to associated stream,
       closes the stream.
     */

    stream = fopen ("/tmp/journal.txt", "a");
    if (!stream) {
        perror("fopen");
        return -1;
    }
    if (fputs("the ship is made of wood.\n", stream) == EOF) {
        perror("fputs");
        return -1;
    }
    if (fclose (stream) == EOF) {
        perror("fclose");
        return -1;
    }

    /* write binary data use fwrite() */

    /* Seeking a Stream 

       int fskeek (FILE *stream, long offset, int whence);

       whence --> SEEK_SET, file position --> offset;
       whence --> SEEK_CUR, file position --> current pos + offset;
       whence --> SEEK_END, file position --> EOF + offset;

       void rewind ((FILE *stream);

       The invocation
            rewind (stream);
       resets the position back to the start of the stream.
       equivalent to:
            fseek (stream, 0, SEEK_SET);


       The ftell() function returns the current stream position of stream

       long ftell (FILE *stream);

     */

    /* to obtain the file descriptor backing a stream, use fileno() 

       int fileno (FILE *stream);
     */
    
    
    stream = fopen ("/tmp/treasures.txt", "w");

    if (!stream) {
        perror("fopen2");
        return -1;
    }

    /* ensure the lines are written w/o interleaving write operations 
       from other threads */
    flockfile (stream);
    fputs ("list of treasures:\n", stream);
    fputs ("    (1) 500 gold coins\n", stream);
    fputs ("    (2) Wonderfully ornate dishware\n", stream);
    funlockfile (stream);
    /* ideally, an application is designed such that multiple threads are not 
       submittting io to the same stream. if your app does need to do so, 
       you need an atomic region greater than a single functin, flock can save
       the day. */

    if (fclose (stream) == EOF) {
        perror("fclose");
        return -1;
    }


}

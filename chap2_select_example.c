#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TIMEOUT 5       // select timeout in seconds
#define BUF_LEN 1024    // read buffer in bytes

int main (void)
{
    struct timeval tv;
    fd_set readfds;
    int ret;

    // wait on stdin for input.
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    // wait up to five seconds
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

    /*
       a call to select() blocks until the given file descriptors 
       are ready to perform i/o, or until an optionally specified timeout
       has elapsed.

       A file descriptor is considered ready if it is  possible
       to  perform the corresponding I/O operation (e.g., read(2)) 
       without blocking.

     */
    ret = select (STDIN_FILENO + 1, //nfds
                  &readfds, // watched if data is available for reading
                  NULL,
                  NULL,
                  &tv);
    if (ret == -1) {
        perror ("select");
        return 1;
    } else if (!ret) {
        printf ("%d seconds elepased.\n", TIMEOUT);
        return 0;
    }

    // test if fd is ready to read w/o blocking
    // FD_ISSET returns nonzero integer if the fd is in the set
    // 0 if it is not.
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        char buf[BUF_LEN + 1]; // string length \0
        int len;

        // guaranteed to not block
        len = read (STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            perror("read");
            return 1;
        }

        if (len) {
            buf[len] = '\0'; // add \0 at the end to make it a string
            printf ("read: %s\n", buf); // read the buffer
        }

        return 0;
    }

    fprintf (stderr, "This should not happen!\n");
    return 1;
}

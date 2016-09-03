/**
   check whether read from stdin and write to stdout will block.

   $ gcc chap2_poll_example.c -o poll
   $ ./poll 
   stdout is writable.
   $ ./poll < test4
   stdin is readable.
   stdout is writable.

 */

#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#define TIMEOUT 5

int main(void)
{
    struct pollfd fds[2];
    int ret;

    /* watch stdin for intput */
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    /* watch stdout for ability to write (almost always true) */
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;

    /* all set. block */
    ret = poll (fds, 2, TIMEOUT * 1000);
    if (ret == -1) {
        perror ("poll");
        return 1;
    }

    if (!ret) {
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    if (fds[0].revents & POLLIN)
        printf ("stdin is readable.\n");
        
    if (fds[1].revents & POLLOUT)
        printf ("stdout is writable.\n");

    return 0;
}

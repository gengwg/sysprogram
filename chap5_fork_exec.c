#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>  // for printf
#include <stdlib.h> // EXIT_FAILURE

int main(void) {

    pid_t pid;

    pid = fork();

    if(pid == -1)
        perror ("fork");

    /* fork() to crate a new process, then the child executes a new binary image */
    /* e.g. a shell running a new program for the user */
    if(!pid) {
        char *args[] = { "ls", NULL };
        // no const in front

        int ret;

        ret = execv ("/bin/ls", args);
        if (ret == -1) {
            perror ("execv");
            exit (EXIT_FAILURE);
        }
    }

    /* the parent process continues running with no change */

    /* Put the parent to sleep for 2 seconds--let the child finished executing */
    wait (2);
    printf( "Finished executing the parent process\n"
            " - the child won't get here--you will only see this once\n" );

}

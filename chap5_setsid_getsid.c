#include <unistd.h>
#include <stdlib.h> // error: ‘EXIT_SUCCESS’ undeclared (first use in this function)
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (void)
{

    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror ("fork");
        return -1;
    } else if (pid !=0 )
        exit (EXIT_SUCCESS);

    if (setsid () == -1) {
        perror ("setsid");
        return -1;
    }

    pid_t sid;

    /* returns the ssion ID of the process identified by pid.
       if the pid argument is 0, returns the ssion id of the calling process.
     */
    sid = getsid (0);

    if (sid == -1)
        perror ("getsid");
    else {
        printf ("My session id=%d\n", sid);
    }

    return 0;

}

/*
   $ ./a.out 
   My session id=24702
 */

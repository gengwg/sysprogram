#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

/* this program forks a child, which immediately exits.
   the parent proccess executes the wait() system call to determine
   the status of the child.
   the process prints the child's pid, and how it died.
   in this case the child terminated by returning from main()
 */

int main (void)
{
    int status;
    pid_t pid;

    if (!fork ())
        return -1;

    /* a call to wait() returns the pid of a terminated child or -1 on error */
    pid = wait (&status);
    if (pid == -1)
        perror ("wait");

    printf ("pid=%d\n", pid);

    if (WIFEXITED (status))
        printf ("Normal termination with exit status=%d\n",
                WIFEXITED (status));

    if (WIFSIGNALED (status))
        printf ("Killed by signal=%d%s\n",
                WTERMSIG (status),
                WCOREDUMP (status) ? " (dumped core)" : "");

    if (WIFSTOPPED (status))
        printf ("Stopped by signal=%d\n",
                WSTOPSIG (status));

    if (WIFCONTINUED (status))
        printf ("Continued\n");

    return 0;

}

/* Output: 
   $ ./a.out 
   pid=31368
   Normal termination with exit status=1
 */

#include <unistd.h>
#include <stdlib.h> // error: ‘EXIT_SUCCESS’ undeclared (first use in this function)
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <linux/fs.h>

#define NR_OPEN         1024

int main (void)
{

    pid_t pid;
    int i;

    /* create new process */
    pid = fork();
    if (pid == -1) {
        perror ("fork");
        return -1;
    } else if (pid !=0 )
        exit (EXIT_SUCCESS);

    /* create new session and process group */
    if (setsid () == -1) {
        perror ("setsid");
        return -1;
    }

    /* set the working dir to the root dir */
    if (chdir ("/") == -1)
        return -1;

    /* close all open files -- NR_OPEN is overkill, but works */
    for (i = 0; i < NR_OPEN; i++)
        close (i);

    /* redirect fd's 0,1,2 to /dev/null */
    open ("/dev/null", O_RDWR); /* stdin */
    dup (0);                    /* stdout */
    dup (0);                    /* stderror */

    return 0;

    daemon(0, 0);
}

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>  // O_RDONLY
#include <stdlib.h> // EXIT_FAILURE
#include <stdio.h> 

#define BUF_LEN 1024

/* this is how the shells implment caching of previous directory
   e.g. cd - in bash.
 */
int main()
{
    int swd_fd, ret;
    char *some_other_dir = "/tmp";

    /* open the cwd so that process can return to it later */
    swd_fd = open(".", O_RDONLY);
    if (swd_fd == -1) {
        perror ("open");
        exit (EXIT_FAILURE);
    }

    /* change to a differnet dir */
    ret = chdir (some_other_dir);
    if (ret) {
        perror ("chdir");
        exit (EXIT_FAILURE);
    }

    /* print the current working directory */
    char cwd[BUF_LEN];

    if (!getcwd (cwd, BUF_LEN)){
        perror ("getcwd");
        exit (EXIT_FAILURE);
    }

    printf ("cwd = %s\n", cwd);

    /* return to the saved directory */
    ret = fchdir (swd_fd);
    if (ret) {
        perror ("chdir");
        exit (EXIT_FAILURE);
    }

    /* close the directory fd */
    ret = close(swd_fd);
    if (ret) {
        perror ("close");
        exit (EXIT_FAILURE);
    }

    /* print the current working directory */
    if (!getcwd (cwd, BUF_LEN)){
        perror ("getcwd");
        exit (EXIT_FAILURE);
    }

    printf ("cwd = %s\n", cwd);


}

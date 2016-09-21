#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> // for printf

int main(void) {

    pid_t pid;

    pid = fork();

    /*You're actually seeing the output of two copies of the executable. When you call fork(),
      the process clones itself, giving two processes. The parent gets the child process's PID 
      as the return value, and the child gets 0 as a return value.
     
      The child and the parent process are identical in almost every facet.

      The trick is, the clones share STDIN, STDOUT, and STDERR. When the parent reaches 
      the printf, it prints the value it had, as does the child, so you see both PIDs, 
      with both processes sharing the same STDOUT -- there's no obvious way to tell them apart.
     */
    if(pid > 0)
        printf ("I am the parent of pid=%d!\n", pid);
    else if(!pid)
        printf ("I am the child of pid=%d!\n", pid);
    else if(pid == -1)
        perror ("fork");
}

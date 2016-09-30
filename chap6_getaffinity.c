#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (void)
{
    cpu_set_t set;
    int ret, i;

    /* before invocation zero out all bits in the set */
    CPU_ZERO (&set);
    /* sched_getaffinity() retrieves the CPU affinity of the proces 
       if pid is zero, retrieves current process's affinity. */
    ret = sched_getaffinity(0, sizeof (cpu_set_t), &set);
    if (ret == -1)
        perror ("sched_getaffinity");

    for (i = 0; i < CPU_SETSIZE; i ++) {
        int cpu;

        cpu = CPU_ISSET (i, &set);
        printf ("cpu=%i is %s\n", i,
                cpu ? "set" : "unset");
    }

}

/* output 
   $ ./a.out  | head
   cpu=0 is set
   cpu=1 is set
   cpu=2 is set
   cpu=3 is set
   cpu=4 is unset
   cpu=5 is unset
   cpu=6 is unset
   cpu=7 is unset
   cpu=8 is unset
   cpu=9 is unset
 */

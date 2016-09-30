#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void)
{
    cpu_set_t set;
    int ret, i;

    CPU_ZERO (&set);    // clear all CPUs
    CPU_SET (1, &set);  // alow CPU #0
    CPU_CLR (0, &set);  // disallow CPU #1
    /* the CPU_CLR operation is redundant as we just zeroed out the whole set
       but it's provided for completeness
    */

    ret = sched_setaffinity(0, sizeof (cpu_set_t), &set);
    if (ret == -1)
        perror ("sched_setaffinity");

    for (i = 0; i < CPU_SETSIZE; i ++) {
        int cpu;

        cpu = CPU_ISSET (i, &set);
        printf ("cpu=%i is %s\n", i,
                cpu ? "set" : "unset");
    }

}

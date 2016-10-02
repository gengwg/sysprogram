#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

/* The ulimit shell command is a wrapper around the setrlimit system call,
so that's where you'll find the definitive documentation. */

int main (void)
{

    struct rlimit rlim;
    int ret;

    /* get the limit on core sizes */
    ret = getrlimit (RLIMIT_CORE, &rlim);
    if (ret == -1) {
        perror ("getrlimit");
        return 1;
    }

    printf ("RLIMIT_CORE limits: soft=%ld, hard=%ld\n",
            rlim.rlim_cur, rlim.rlim_max);

    /* sets max core size to 32 MB */
    rlim.rlim_cur = 32 * 1024 * 1024;   /* 32 MB */
    rlim.rlim_max = RLIM_INFINITY;   /* same */
    ret = setrlimit (RLIMIT_CORE, &rlim);
    if (ret == -1) {
        perror ("setrlimit");
        return 1;
    }

    printf ("RLIMIT_CORE limits: soft=%ld, hard=%ld\n",
            rlim.rlim_cur, rlim.rlim_max);

}

/* output:
./a.out
RLIMIT_CORE limits: soft=0, hard=-1
(We have a soft limit of 0, and a hard limit of infinity. -1 denotes RLIM_INFINITY)
RLIMIT_CORE limits: soft=33554432, hard=-1
 */

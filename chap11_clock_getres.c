#include <time.h>
#include <stdio.h>

int main (void)
{
    clockid_t clocks[] = {
        CLOCK_REALTIME,
        CLOCK_MONOTONIC,
        CLOCK_PROCESS_CPUTIME_ID,
        CLOCK_THREAD_CPUTIME_ID,
        CLOCK_MONOTONIC_RAW,
        (clockid_t) -1};

    int i;

    for (i = 0; clocks[i] != (clockid_t) -1; i++) {
        struct timespec res;
        int ret;

        ret = clock_getres (clocks[i], &res);
        if (ret)
            perror ("clock_getres");
        else
            printf ("clock=%d sec=%ld nsec=%ld\n",
                    clocks[i], res.tv_sec, res.tv_nsec);
    }

    return 0;
}

/* POSIX clocks requires linking the resulting object file with librt.
   $ gcc -Wall -W -O2 -lrt -g chap8_clock_getres.c
   $ ./a.out 
   clock=0 sec=0 nsec=1
   clock=1 sec=0 nsec=1
   clock=2 sec=0 nsec=1
   clock=3 sec=0 nsec=1
   clock=4 sec=0 nsec=1
 */

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* memset */

int set_highest_priority (pid_t pid) 
{
    struct sched_param sp;
    int policy, max, ret;

    policy = sched_getscheduler (pid);
    if (policy == -1)
        return -1;

    max = sched_get_priority_max (policy);
    if (max == -1) {
        perror ("sched_get_priority_max");
        return 1;
    }

    memset (&sp, 0, sizeof (struct sched_param));
    sp.sched_priority = max;

    ret = sched_setparam (pid, &sp);

    return ret;
}
int main (void)
{

    struct sched_param sp = { .sched_priority = 1 };
    int ret;

    /* setting a scheudling policy other than SCHED_OTHER 
       requires CAP_SYS_NICE capablity. Consequently,
       the root user typically runs a real-time process.
     */

    //ret = sched_setscheduler (0, SCHED_FIFO, &sp);
    ret = sched_setscheduler (0, SCHED_RR, &sp);
    if (ret == -1) {
        perror ("sched_setscheduler");
        return 1;
    }

    int policy;

    /* get our scheduling policy */
    policy = sched_getscheduler (0);

    switch (policy) {
    case SCHED_OTHER:
        printf ("Policy is normal\n");
        break;
    case SCHED_RR:
        printf ("Policy is round-robin\n");
        break;
    case SCHED_FIFO:
        printf ("Policy is first-in, first-out\n");
        break;
    case -1:
        perror ("sched_getscheduler");
        break;
    defaut:
        fprintf (stderr, "unknown policy!\n");
    }

    /* get our current priority */
    printf ("Our priority is %d\n", sp.sched_priority);

    /* set our priority to a number */
    sp.sched_priority = 5;
    ret = sched_setparam (0, &sp);
    if (ret == -1) {
        perror ("sched_setparam");
        return 1;
    }

    ret = sched_getparam (0, &sp);
    if (ret == -1) {
        perror ("sched_getparam");
        return 1;
    }

    printf ("Our priority is %d\n", sp.sched_priority);

    /*check priority range for policy SCHED_RR*/
    int min, max;

    min = sched_get_priority_min (SCHED_RR);
    if (min == -1) {
        perror ("sched_get_priority_min");
        return 1;
    }

    max = sched_get_priority_max (SCHED_RR);
    if (max == -1) {
        perror ("sched_get_priority_max");
        return 1;
    }

    printf ("SCHED_RR priority range is %d - %d\n", min, max);

    /* set our priority to highest */
    ret = set_highest_priority (0);
    if (ret == -1) {
        perror ("set_highest_priority");
        return 1;
    }

    ret = sched_getparam (0, &sp);
    if (ret == -1) {
        perror ("sched_getparam");
        return 1;
    }

    printf ("Our priority is %d\n", sp.sched_priority);


    /*retrieving length of a given process's timeslice*/
    struct timespec tp;

    /* according to POSIX, this function is required to work only with SCHED_RR processes.
       on linux, it can retrive lenght of any process's timeslice.
     */
    ret = sched_rr_get_interval (0, &tp);
    if (ret == -1) {
        perror ("sched_rr_get_interval");
        return 1;
    }

    printf ("Our time quantum is %.2lf milliseconds\n",
            (tp.tv_sec * 1000.0f) + (tp.tv_nsec / 1000000.0f));

    /* if the process is running in the FIFO class, tv_sec and tv_nsce are both 0,
       denoting infinity.
     */
}

/* output:
   $ ./a.out 
   Policy is normal

   $ ./a.out 
   sched_setscheduler: Operation not permitted
   $ sudo ./a.out 
   Policy is round-robin

   $ sudo ./a.out 
   Policy is round-robin
   Our priority is 1
   Our priority is 5
   SCHED_RR priority range is 1 - 99
   Our priority is 99
   Our time quantum is 100.00 milliseconds
 */

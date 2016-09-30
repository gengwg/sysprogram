#include <unistd.h> // nice()
#include <stdio.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/resource.h>

int main (void)
{
    int ret;

    /* because nice returns the new nice value, -1 is also a succesful return value.
       to diff, one can zero out errno before invocation and subsequently check its value.
     */
    errno = 0;
    /* int nice (int inc)*/
    ret = nice (5); // increase nice by 5

    if (ret == -1 && errno != 0) {
        perror ("nice");
        return -1;
    } else 
        printf ("nice value is now %d\n", ret);

    int val;

    /* get current nice value */
    val = nice (0);

    /* set absolute nice value of 10 */
    val = 10 - val;
    errno = 0;
    ret = nice (val);

    if (ret == -1 && errno != 0) {
        perror ("nice");
        return -1;
    } else 
        printf ("nice value is now %d\n", ret);

    ret = setpriority (PRIO_PGRP, 0, 11);
    if (ret == -1)
        perror ("setpriority");

    ret = getpriority (PRIO_PROCESS, 0);
    printf ("nice value is %d\n", ret);

    return 0;

}

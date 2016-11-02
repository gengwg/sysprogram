#include <time.h>
#include <sys/time.h>
#include <stdio.h>

int main (void)
{
    /* simplest 
       time() returns the current time represented as number of seconds 
       elapsed since the epoch.
     */
    time_t t;

    printf ("current time: %ld\n", (long) time (&t));
    printf ("the same value: %ld\n", (long) t);

    /* gettimeofday() extends time() by offerring microsecond resolution
     */
    struct timeval tv;
    int ret;

    /* timezone structure and tz parameter are obsolte. 
       always pass NULL for tz. because kernel does not
       manage time zone, glibc refuese to use timezone.
     */
    ret = gettimeofday (&tv, NULL);
    if (ret)
        perror ("gettimeofday");
    else
        printf ("seconds=%ld useconds=%ld\n",
                tv.tv_sec, tv.tv_usec); // book has typo here tv.sec


    return 0;
}

/* POSIX clocks requires linking the resulting object file with librt.
   $ gcc -Wall -W -O2 -lrt -g chap8_get_current_time.c
   $ ./a.out 
   current time: 1477979565
   the same value: 1477979565
   seconds=1477979565 useconds=126594

 */

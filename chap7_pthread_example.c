#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
//#include <unistd.h>

void * start_thread (void *message)
{
    // printf is thread safe
    printf ("%s\n", (const char *) message);
    return message;
    //return 0;
}

int main (void)
{
    pthread_t thing1, thing2;
    const char *message1 = "Thing 1";
    const char *message2 = "Thing 2";

    /* create 2 threads in same start routing, with diff messages.
     */
    pthread_create (&thing1, NULL, start_thread, (void *) message1);
    pthread_create (&thing2, NULL, start_thread, (void *) message2);

    /* wait for the threads to exit. if we didn't join here
       we risk terminating this main thread before the other
       two threads finished, terminating the whole process.
     */
    pthread_join (thing1, NULL);
    pthread_join (thing2, NULL);

    //sleep (1);
    return 0;
}
/* usage:
   $ gcc -Wall -O2 -pthread chap7_pthread_example.c 

   must use -pthread, otherwise:
   chap7_pthread_example.c:(.text+0x6c): undefined reference to `pthread_create'

   $ ./a.out 
   Thing 1
   Thing 2
   $ ./a.out 
   Thing 2
   Thing 1

   if no join:
   $ ./a.out 
   Thing 1
   Thing 1
   $ ./a.out 
   TThing 1
   ./a.out 
   TThing 1
   Thing 2
   ./a.out 
   Thing 1
   ./a.out 
   $ ./a.out 
   Thing 1
   TT

 */

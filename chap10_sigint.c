#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#define _GNU_SOURCE
#include <string.h>

/* handler for SIGINT */
static void sigint_handler (int signo)
{
    printf ("Caught SIGINT!\n");
    //printf ("Caught %s\n", strsignal[signo]);
    exit (EXIT_SUCCESS);
}

int main (void)
{
    /* register signal_handler as our signal handler for SIGINT */
    if (signal (SIGINT, sigint_handler) == SIG_ERR) {
        fprintf (stderr, "Cannot handle SIGINT!\n");
        exit (EXIT_FAILURE);
    }

    for (;;)
        pause ();

    return 0;
}
/*
   In one terminal,
   $ ./a.out 

   In another terminal,
   $ kill -s SIGINT 21706

   One will see in ther first terminal:
   Caught SIGINT!
 */

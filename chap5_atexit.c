#include <stdio.h>
#include <stdlib.h>

void out (void)
{
    printf ("atexit() succeeded!\n");
}

int main (void)
{
    // on success, atexit() returns 0;
    // on error, it returns -1.
    if (atexit (out))
        fprintf(stderr, "atexit() failed!\n");

    return 0;
}

/* output:
 $ ./a.out 
 atexit() succeeded!
 */

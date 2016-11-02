#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main (void)
{

    int ret;

    ret = kill (22495, SIGTERM);
    if (ret) 
        perror ("kill");

    return 0;
}

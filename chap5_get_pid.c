#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> // for printf

// must include this to use intmax_t 
#include <inttypes.h>

int main(void) {

    //pid_t getpid (void);
    //pid_t getppid (void);

    printf ("My pid=%jd\n", (intmax_t) getpid ());
    printf ("Parent's pid=%jd\n", (intmax_t) getppid ());

    // if system lacks intmax_t, one can use the default int pid_t
    printf ("My pid=%jd\n", getpid ());
    printf ("Parent's pid=%jd\n", getppid ());
}

/* Output:
   My pid=12241
   Parent's pid=3614 // this is pid of your current shell
   My pid=12241
   Parent's pid=3614
 */

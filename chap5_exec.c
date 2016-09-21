#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> // for printf

/* replaces the currently executing program with /bin/vi */

int main(void) {

    int ret;

    /* a call to execl() replaces the current process image with a new one by loading into
       memory the program pointed at by path. the parameter arg is the first argument to 
       this program. the excell() function is valriadic, that additional arguments may 
       optionally follow. The list of args must beNULL-terminated.
     */
    ret = execl ("/bin/vi", "asdfsdf", "./tt", "asdf", NULL);
    /* here we follow the Unix convention and pass "vi" as the program's first arg.
       the shell puts the last component of the path, vi, into the first argument when it 
       forks/execs process, so a prgram so a program can examinit's first argument, argv[0],
       to discover the name of it's binary image.

       in many cases, several system utilties that appear as different names to the user are 
       in fact a single program with hard links for their multiple names. the program uses
       the 1st argument to determine its behavior.
     */
    // ret = execl ("/bin/vi", NULL);
    if (ret == -1)
        perror ("excel");
}

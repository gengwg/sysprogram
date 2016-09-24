# sysprogram
system programing

## Terminating a program

```
#include <stdlib.h>

void exit (int status);
```

exit() performs some basic shutdown steps, then instructs the kernel to terminate the process.

status & 0377 is returned to the parent.

`EXIT_SUCCESS` and `EXIT_FAILURE` are defined as portable ways to represent success/failure.
On linux, 0 typically success; 1 or -1 failure.
e.g.

    exit (EXIT_SUCCESS_);

Before terminating the process, the C lib performs the following shutdown steps, in order:

1. call any functions registered with `atexit()` or `on_exit()`.

2. flush all open standard i/o streams.

3. remove any temporary files created with the tmpfile() function.

these steps finish all the work the process need to do in user space, so exit() invokes
the system call `_exit()` to let the kernel handle the rest of the termination process:

```
# include <stdlib.h>

void _exit (int status);
```

when process exits, the kernel cleans up all of the resources that it created on the process's behalf
that are no longer in use. includes, allocated memory, open files, and system V semaphores.
after cleanup, the kenerl destroys the process and notifies the parent of its child's demise.



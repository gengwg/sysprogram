# sysprogram
system programing

## Chapter 5

### Terminating a Process

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

### Other ways to terminate

"Falling off the end". in the case of C/C++, this happens when the main() function returns. "Falling off the end"
approach still invokes a system call: the compiler simply inserts and implicit call to `exit()`
after its own shutdown code. it's good coding practice to explicitly return an exit status,
either via exit(),  or by reutrning a vlaue from main().
note that a successful return is exit(0), or a return from main() of 0.

a process and also terminate if it is sent a signal whose default action is to terminate the process.
such includes SIGTERM and SIGKILL.

final way to end a program's execution is by incurrng the wrath of the kernel. 
the kernel can kill a process for executing an illegal instruction, causing a segmentation violation, running out of memory, consuming moreresources that allowed, and so on.

### atexit()

```
#include <stdlib.h>

int atexit (void (*function)(void));
```

registers the given function to run during normal process termination, 
i.e. when a process is terminated via either exit() or a return from main().

if a process invokes an exec funtion, the list of registered functions is cleared 
(as the functions no longer exist in the new process's address space).
if a process terminates via a signal, the registered functions are not called.

the given function takes no parameters and returns no value.
    
    void my_function (void);

functions are invoked in the reverse order that they are registered. 
they are stored in a stack, and LIFO.

Registered functions must not call exit() lest they begin an endless recursion.

[Example](chap5_get_pid.c "atexit").

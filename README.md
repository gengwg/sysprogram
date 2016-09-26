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

Here is an [Example](chap5_get_pid.c "atexit").

When a process terminates, the kernel sends the signal SIGCHLD to the parent.
By default, this signal is ignored, and no action is taken by the parent.
Processes and elect to handle this signal via signal() or sigaction() system calls.

### Wait for terminated child processes

when a child dies before its parent, kernel should put the child into a special process state, zombie. only some basic kernel data structures containing potentially useful data is retained. a process in this state waits for its parent to inquire about its status. only after the parent obtains the informatin preserved about the terminated child does the process formally exit and cease to exist even as a zombie.

if your application forks a child process, it is your responsiblitiy to wait on the child, even if it will merely discard the info gleaned. otherwise, all of your process's children will become ghosts and live on, crowding the system's process listing.

whenever a process terminates, the kernel walks a list of its children and reparents all of them to the init process with pid value 1. the init process periodically waits on all of its children, ensuring none remain zombies for too long.

There are 4 user IDs associated with a process:
real, effective, saved, and filesystem.

1. the real user ID is hte uid of the user who originally ran the process. 
normally the login process sets the realy user ID of the user's login shell to that of hte user, and all of the user's process continue to carry this user ID.

2. the effective user ID is the user ID that the process is currently wielding.
Permission verifications normally check against this value. 
by executing a setuid binary, the process can change its effective user id.
the effective id is set to the user id of hte owner of the program file.

3. the saved user id is hte process's original effective user ID.

### Sessions and process groups

the process group id is equal to the pid of the process group leader.

a session is a collection of one or more process groups. sessions arrange a logged-in user's activities nad associate that user wiht a controlling terminal.
sessions are largely the business of shells.

sessions exist to consolidate logins around contorlling terminals.
process groups in a session are divided into a single foreground process group and zero or more backgroud process groups..
when a user exits a terminal, a SIGQUIT is sent to all processes in the foreground processs group.
when a network disconnect is detected by a terminal, a SIGHUP is sent to all processes in the foreground processs group.
when the interrupt key (Ctrl+C), a SIGINT is sent to all processses in the foreground processs group.
sessions make managing terminals and logins easier for shells.

on a given system, there are many sessions: one for each user login and others for processes not tied to user logins, such as daemons. daemons tend to create their own sessions to avoid the issues of assocation with other sessions that may exit.

the easiest way to ensure that any given process is not a process group leader is to fork, have the parent terminate, and have the child preform the setsid().

Here is an [Example](chap5_setsid_getsid.c "session system calls").

### Daemons

a daemon is a process that runs in the background, not connecting to any controlling terminal. daemons are normally started at boot time, run as root
or some other special user (apache, postfix), and handle system-level tasks.

a daemon has two general requirements: it must run as a child of init nad it must not be connected to a terminal.

a program performas following steps to become a daemon:

1. call fork(). this creates the new process which will become the daemon.

2. in the parent, call exit(). this ensures that the orignal parent is no longer runing.

3. call setsid(), givine the daemon a new process group and session, both of which have it as leader.

4. change working directory to the root dir via chdir(). the inherited working dir can be anywhere on the filesystem.

5. close all file descritors. same inherit above.

6. open file descriptors 0, 1, and 2. and redirect them to /dev/null.

Here is an [Example](chap5_daemonize.c "Daemons").




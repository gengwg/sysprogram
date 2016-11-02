# sysprogram
system programing

## Chapter 5

### Terminating a Process

```c
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

```c
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


## Chapter 6 - Advanced Process Management

### The Completely Fair Scheduler

A runnable process is one that is not blocked; a blocked process is one that is sleeping, waiting for I/O from the kernel.

Deciding which processes run, when, and for how long is the process scheduler’s fundamental responsibility.

Multitasking operating systems come in two variants: cooperative and preemptive. Linux
implements the latter form of multitasking, where the scheduler decides when one pro‐
cess is to stop running and a different process is to resume. We call the act of suspending
a running process in lieu of another one preemption. The length of time a process is
allowed to run before the scheduler preempts it is known as the process's timeslice, so
called because the scheduler allocates the process a "slice" of the processor's time.

Processes that continually consume all of their available timeslices are considered
processor-bound.
examples include scientific computations, mathematical calculations, and image processing.

processes that spend more time blocked waiting for some resource
than executing are considered I/O-bound. I/O-bound processes are often issuing and
waiting for file or network I/O, blocking on keyboard input, or waiting for the user to
move the mouse. Examples of I/O-bound applications include file utilities that do very
little except issue system calls asking the kernel to perform I/O, such as cp or mv, and many GUI applications, which spend a great deal of time waiting for user input.

By assigning proportions of the processor and not fixed timeslices, CFS is able to enforce
fairness: each process gets its fair share of the processor. Moreover, CFS is able to enforce
a configurable scheduling latency, as the target latency is user-settable. On traditional
Unix schedulers, processes run for fixed timeslices known a priori, but the scheduling
latency (how often they run) is unknown. On CFS, processes run for proportions and
with a latency that is known a priori, but the timeslice is dynamic, a function of the
number of runnable processes on the system. It is a markedly different way of handling
process scheduling, solving many of the problems around interactive and I/O-bound
processes that has plagued traditional process schedulers.

### I/O priorities

I/O priorities vs scheduling priority

by default, kernel's I/O scheduler use a process's nice value to dtermine the i/o priority.

glibc does not provide a user-space interface to the i/o priority sys calls.
use nice value, or a utility such as ionice, part of until-linux package.

### CPU affinity and RT process

This grabs init's current set of allowed processors, which is all of them.
it then removes one processor, CPU #1, from the set and updates the list of
allowed processors.

```c
cpu_set_t set;
int ret;

CPU_ZERO (&set);  /* clear all CPUs */
ret = sched_getaffinity (0, sizeof (cpu_set_t), &set);
if (ret == -1){
  perror("sched_getaffinity");
  return 1;
}

CPU_CLR (1, &set); /* forbid CPU #1 */
ret = sched_setaffinity (0, sizeof (cpu_set_t), &set);

```

because set of allowed processorsis inherited from parent to child,
and init is hte super-parent of all processes, all of the system's processes will run with this set of allowed processors.

Next modify the RT process to run only on CPU #1:

```c
cpu_set_t set;
int ret;

CPU_ZERO (&set);  /* clear all CPUs */
CPU_SET (1, &set);  /* allow CPU #1*/
ret = sched_setaffinity (0, sizeof (cpu_set_t), &set);
if (ret == -1){
  perror("sched_setaffinity");
  return 1;
}
```

the result is your RT process runs only on CPU #1, and all other processes run on the other processors.

### Resource limits

The hard limit is the ceiling for the soft limit. The soft limit is what is actually enforced for a session or process. This allows the administrator (or user) to set the hard limit to the maximum usage they wish to allow. Other users and processes can then use the soft limit to self-limit their resource usage to even lower levels if they so desire.

### Threading models

kernel-level threading, 1:1 threading. kernel provices native support for threads ,and each of those kernel threads translates directly to the user space concept of a thread.
the linux kernel implements threads simply as processes that share resources. the threading library creates a new thread via the clone() system call, and the returned process is directly managed as the user space concept of a thread.

user-level threading, N:1 threading. a process with N threads will map to a single kernel process.
this model cannot utilize multiple processors and thus is unable to provide true parallelism.

## Chapter 8 File and Directory Management

Linux currently defines 4 extended attribute namespaces:

* system

the system namespace is used to implement kernel features that utilize extended attributes, such as access control lists ACLs.
whether a user can read or write to these attributes depends on the security module in place.

* security 

the security namespace is used to implement  security modules, such as SELinux.

* trusted

stores restricted information in userspace. only processes with `CAP_SYS_ADMIN` capability can read or write to these atributes

* user

standard namespace for regular processes. kernel control access to this namespace via the normal file permission bits.
you can assign xattr in the user name space only to regular files, not to symbolic links or device files.
designing a user space application that uses xatt, this is it.

## Chapter 11 Time

call sleep() returns the number of seconds _not_ sleep. e.g. a signal interrupts the nap.

if sleeping the entire specified time is a concern, you cna continue callling sleep() with its return value, until it returns 0.

```bash
    unsigned int s = 5;

    while ((s = sleep (s)))
        ;
```

they will sleep _at least as long as requested_.



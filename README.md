# Scheduling policies in xv6

This patch of xv6 aims to present 5 different scheduling policies which can be used in xv6.
The 5 policies implemented: DEFAULT, FCFS, PRIORITY, SML and LOTTERY. 
In order to enable a specific policy, when you launch qemu you have to specify the command above, which will set a flag that will enable the scheduling policity specified in it.

```
$ make qemu SCHEDFLAG=FCFS
```

Alternatively if you want to patch your existing system you can launch the following command:

```
$ ./generate.sh --lab lab_scheduling --flags FCFS
```

If the flag isn't defined at launch, then DEFAULT (Round-Robin policy) is used.

## Polices

* [DEFAULT](#default---round-robin) - This is the default Round-Robin policy which comes with the vanilla version of xv6
* [FCFS](#fcfs---first-come-first-served) - **F**irst **C**ome **F**irst **S**erved
* [PRIORITY](#priority---priority-scheduling-algoritm) - Priority algoritm based on priorities values
* [SML](#sml---static-multilevel-queue-scheduling) - **S**tatic **M**ulti­**L**evel queue scheduling
* [LOTTERY](#lottery---lottery-probabilistic-scheduling-algorithm) - Lottery algorithm based on tickets and probability

## DEFAULT - Round Robin

The default algorithm implemented in xv6 it's one of the simplest (with FCFS) and relies on the Round-Robin policy, basically it loops through all the process which are available to run (marked with the ```RUNNABLE```) state and give access to
CPU at each one of them one at a time.
To schedule processes fairly, a round-robin scheduler generally employs time-sharing, giving each job a time slot or quantum (its allowance of CPU time), and interrupting the job if it is not completed by then. 
The job is resumed next time a time slot is assigned to that process. 
If the process terminates or changes its state to waiting during its attributed time quantum, the scheduler selects the first process in the ready queue to execute. 
In the absence of time-sharing, or if the quanta were large relative to the sizes of the jobs, a process that produced large jobs would be favoured over other processes.
Round-robin scheduling is simple, easy to implement, and starvation-free.

To enable it and see how DEFAULT works use this command when compiling xv6:

```
$ make qemu SCHEDFLAG=DEFAULT
```

```
$ ./generate.sh --lab lab_scheduling --flags DEFAULT
```

## FCFS - First Come First Served

First come first served (FCFS), is the simplest scheduling algorithm. FCFS simply queues processes in the order that they arrive in the ready queue. 
The scheduling overhead due to using this policy is minimal since context switches only occur upon process termination, and no reorganization of the process queue is required.
Throughput can be low, because long processes can be holding CPU, waiting the short processes for a long time, so short processes which are in a queue are penalized over the longer ones (known as convoy effect).
By using this policy we have no starvation, because each process gets chance to be executed after a definite time.
Turnaround time, waiting time and response time depends on the order of their arrival and can be high for the same reasons above.
There isn't prioritization, so using this policy we cannot force certain processes to be completed first which means that this system has trouble meeting process deadlines.
The lack of prioritization means that as long as every process eventually completes, there is no starvation. 
In an environment where some processes might not complete, there can be starvation since the processes that come next the one which might not complete are never executed.

To enable it and see how FCFS works use this command when compiling xv6:

```
$ make qemu SCHEDFLAG=FCFS
```

```
$ ./generate.sh --lab lab_scheduling --flags FCFS
```

## PRIORITY - Priority scheduling algoritm

The priority scheduling algorithm (SML) represents a preemptive policy that executes processes based on their priority.
The scheduling policy first select the runnable process with the lowest value of priority and executes it, after that it finds the one with the seconds lowest value of priority and excutes it
and so on, until we have finished all the processes.
This scheduling policy allows the user to mark some processes which we wants to be completed first in a simple but fast way.
Priority range in this algorithm is 1-20 (default is 10) where we give priority equals to 1 for the processes which we want to be completed first.

The following system call will change the priority queue of the process with a specific pid process:

```
int chpr(int pid, int priority)
```

In this case ```priority``` is a number between 1 and 20 which represents the new process priority.

To enable it and see how PRIORITY works use this command when compiling xv6:

```
$ make qemu SCHEDFLAG=PRIORITY
```

```
$ ./generate.sh --lab lab_scheduling --flags PRIORITY
```


## SML - Static multilevel queue scheduling

The static multilevel queue scheduling (SML) represents a preemptive policy that includes a three priority queues (priority can asusme three values: 1,2 and 3). 
The initial process should be initiated at priority 2 and the priority should be copied upon fork. 
In this scheduling policy the scheduler will select a process from a lower queue only if no process is ready to run at a higher queue.
The algorithm first runs all the process with highest priority and then, when they finish, it will consider all the process with a lower priority.
Moving between priority queues is only available via a system call.
This algorithm is very similar to PRIORITY, but in this case we have only three queues (low, medium, high) and the user must select foreach process which queue the process belongs to (default -> 2: medium)
Priority range in this algorithm is 1-3 (default is 2) where we give priority equals to 1 for the processes which we want to be completed first.

The following system call will change the priority queue of the process with a specific pid process:

```
int chpr(int pid, int priority)
```

In this case ```priority``` is a number between 1 and 3 which represents the new process priority.

To enable it and see how SML works use this command when compiling xv6:

```
$ make qemu SCHEDFLAG=SML
```

## LOTTERY - Lottery probabilistic scheduling algorithm

The lottery is a probabilistic scheduling algorithm where at each process are each assigned some number of lottery tickets and the scheduler draws a random ticket to select the next process to run. 
The distribution of tickets need not be uniform; granting a process more tickets provides it a relative higher chance of selection. This technique can be used to approximate other scheduling algorithms, such as Shortest job next and Fair-share scheduling.
Lottery scheduling solves the problem of starvation. Giving each process at least one lottery ticket guarantees that it has non-zero probability of being selected at each scheduling operation.
A straightforward way to implement a centralized lottery scheduler is to randomly select a winning ticket, and then search a list of clients to locate the client holding that ticket. 
This requires a random number generation and O(n) operations to traverse a client list of length n, accumulating a running ticket sum until it reaches the winning value.

The following system call will change the tickets of the process with a specific pid process:

```
int chtickets(int pid, int tickets)
```

In this case ```tickets``` is a number between 1 and 100 which represents the new process' tickets.

Example:

![alt Lottery scheduling algorithm example](https://image.prntscr.com/image/aAn5ZSaWTbuTZIRISLnaFw.png)

*Five clients compete in a list-based
lottery with a total of 20 tickets. The fifteenth ticket is randomly
selected, and the client list is searched for the winner. A running
ticket sum is accumulated until the winning ticket value is reached.
In this example, the third client is the winner.*

Additional information on Lottery algorithm can be found here: [lottery paper](https://www.usenix.net/legacy/publications/library/proceedings/osdi/full_papers/waldspurger.pdf)

To enable it and see how LOTTERY works use this command when compiling xv6:

```
$ make qemu SCHEDFLAG=LOTTERY
```

```
$ ./generate.sh --lab lab_scheduling --flags LOTTERY
```



This patch also includes a new system call, similar to ```wait```, but with more funtionalities, in order to check the performances of our scheduling algorithms.
The new system call is called ```wait2``` and returns the creation time, the running time (```rutime```), the sleeping time (```stime```) snd the ready time (runnable) (```retime```)
for each process, this helps a lot in understanding how a scheluding policy affects the times of every process.
For example using the PRIORITY algorithm the sleeping time of processes with a low priority (the one which we need to run first) will be smaller than the one with high priority 
(the one which we want to run later)

Two other important functions which can be useful to play with are

## Authors

* **Marco Fontana** - *Added fcfs, sml and lottery scheduling algorithms, edited nice, added wait2 and reorganization of all scheduling polices (whith descriptions and optimizations)* - [marf](https://github.com/marf)
* **Nicola Bicocchi** - *Command ps, first version of nice and processes priorities* - [os_unimore](https://github.com/nbicocchi/operatingsystemsmsc)


## License

This project is licensed under the MIT License

## Acknowledgments

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also http://pdos.csail.mit.edu/6.828/2014/xv6.html, which
provides pointers to on-line resources for v6.

xv6 borrows code from the following sources:
    JOS (asm.h, elf.h, mmu.h, bootasm.S, ide.c, console.c, and others)
    Plan 9 (entryother.S, mp.h, mp.c, lapic.c)
    FreeBSD (ioapic.c)
    NetBSD (console.c)

The following people have made contributions:
    Russ Cox (context switching, locking)
    Cliff Frey (MP)
    Xiao Yu (MP)
    Nickolai Zeldovich
    Austin Clements

In addition, we are grateful for the bug reports and patches contributed by
Silas Boyd-Wickizer, Peter Froehlich, Shivam Handa, Anders Kaseorg, Eddie
Kohler, Yandong Mao, Hitoshi Mitake, Carmi Merimovich, Joel Nider, Greg Price,
Eldar Sehayek, Yongming Shen, Stephen Tu, and Zouchangwei.

The code in the files that constitute xv6 is
Copyright 2006-2017 Frans Kaashoek, Robert Morris, and Russ Cox.

#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

/*
 * Subset of struct proc (from proc.h)
 * Passing the whole struct proc from kernel
 * to user space would be VERY UNSAFE!
 */
enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct proc {
  enum procstate state;        // Process state
  int pid;                     // Process ID
  int ppid ;                   // Parent process ID
  int priority;                // Process priority
  int tickets;                // Process totalTickets
  int ctime;
  char name[16];               // Process name
};

int
main(int argc, char *argv[]){
  struct proc ptable[NPROC];
  struct proc *p;
  int err;

  err = getptable(NPROC * sizeof(struct proc), &ptable);
  if (err) {
    printf(1,"Error getting ptable");
    exit();
  }

  printf(1, "PID\t\tPPID\t\tSTATE\t\tPRIORITY\tTICKETS\t\tCPUTIME\t\tCMD\n");
  for(p = ptable; p != &ptable[NPROC-1]; p++) {
        if (p->state == UNUSED) continue;

	// init does not have ppid!
	if (p->pid == 1)
	    printf(1, "%d\t\tN/A",p->pid);
	else
	    printf(1, "%d\t\t%d", p->pid, p->ppid);

  	switch(p->state){
  	case UNUSED:
  		printf(1,"\t\t%s", "UNUSED  ");
  		break;
  	case EMBRYO:
  		printf(1,"\t\t%s", "EMBRYO  ");
  		break;
  	case SLEEPING:
  		printf(1,"\t\t%s", "SLEEPING");
  		break;
  	case RUNNABLE:
  		printf(1,"\t\t%s", "RUNNABLE");
  		break;
  	case RUNNING:
  		printf(1,"\t\t%s", "RUNNING ");
  		break;
  	case ZOMBIE:
  		printf(1,"\t\t%s", "ZOMBIE  ");
  		break;
  	}

    printf(1, "\t%d", p->priority);
    printf(1, "\t\t%d", p->tickets);
    printf(1, "\t\t%d", p->ctime);
  	printf(1,"\t\t%s\n", p->name);
  }
  exit();
}

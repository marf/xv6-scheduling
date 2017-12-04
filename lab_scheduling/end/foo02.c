#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define TRUE 1
#define FALSE 0
#define TH 100

int to_be_or_not(int to_be) {
  int count = 0;

  while(count++ < TH) {
    if (to_be) printf(1, "to be!\n");
    if (!to_be) printf(1, "not to be!\n");
    //sleep(1);
  }
  return 0;
}
    
int main(int argc, char **argv) {
  // first child   
  if (fork() == 0) {
    to_be_or_not(TRUE);
    exit();
  }
      
  // second child
  if(fork() == 0) {
    to_be_or_not(FALSE);
    exit();
  }

  // parent
  wait();
  wait();
  exit();
}

  


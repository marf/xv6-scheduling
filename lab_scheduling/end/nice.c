#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  int value, pid;
  char *type;

  if (argc < 3) {
      printf(2, "Usage: nice [type: -p: priority, -t: tickets] [pid] [priority or tickets]\n" );
      exit();
  }
  type = argv[1];
  pid = atoi(argv[2]);
  value = atoi(argv[3]);

  if(strcmp(type, "-p") == 0)
  {
    #ifdef SML
      if (value < 1 || value > 3) {
          printf(2, "Invalid priority (1-3)!\n" );
          exit();
      }
    #else
      if (value < 1 || value > 20) {
          printf(2, "Invalid priority (1-20)!\n" );
          exit();
      }
    #endif

    chpr(pid, value);

  }
  else if(strcmp(type, "-t") == 0)
  {
    if (value < 0 || value > 100) {
        printf(2, "Invalid tickets (0-100)!\n" );
        exit();
    }
    chtickets(pid, value);
  }
  else
  {
    printf(2, "Usage: nice [type: -p: priority, -t: tickets] [pid] [priority or tickets]\n" );
    exit();
  }



  exit();
}

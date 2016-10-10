/* Program: setpriority.c */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[])
{
   int errno, prio;
   pid_t pid;

   if (argc != 3) {
      printf("Usage: %s [pid] [priority (-20~20)]\n", argv[0]);
      return -1;
   }

   pid = atoi(argv[1]);
   prio = atoi(argv[2]);
   //errno = setpriority(PRIO_PROCESS, pid, prio);
   errno = setpriority(PRIO_PROCESS, 0, prio);
   if (errno < 0)
       perror("info");

   printf("OK.\n");

   return 0;
}

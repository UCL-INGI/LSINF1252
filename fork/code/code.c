#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc==1)
    return -1;
  if (argc==2)
    return strlen(argv[1]);
  if(argc==3)
    kill(getpid(), SIGUSR1);
  sleep(1);
  return -1;
}

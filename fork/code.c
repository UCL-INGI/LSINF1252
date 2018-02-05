#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char** argv) {
	// pas d'arguments, exit(-)
	
  	if (argc==1)
    	exit(-1);
    // un argument -> exit(0)    	
  	if (argc==2)
    	exit(0);
        	
    if(argc==3)
      kill(getpid(), SIGUSR1);
  sleep(1);
  return -1;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  
  for (int i=1; i<argc;i++) {
    if((i%2)==1) {
      fprintf(stdout,"%s-",argv[i]);
      fflush(stdout);
    } else {
      fprintf(stderr,"%s-",argv[i]);
      fflush(stderr);
    }
  }
  
  return 0;
}

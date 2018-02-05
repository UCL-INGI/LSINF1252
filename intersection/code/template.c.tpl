#include<stdio.h>
#include<string.h>
#include "student_code.h"

int create(char *filename, int size) {

  int err;
  int fd=open(filename,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
  if(fd==-1) {
    return -1;
  }
  for(int i=1; i<=size; i++) {
    complex val;
    val.r=1;
    val.i=i;
    err=write(fd,(void *) &val, sizeof(complex));
    if(err<0) {
       err=close(fd);
       return(-1);
     }
   }
   err=close(fd);
   if(err==-1)
     return err;
   else
     return size;
 }

int count_same(char *filename1, char *filename2) {
@@intersection@@
}

void montest(void) {

@@montest@@

}

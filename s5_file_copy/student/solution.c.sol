int fd = open(filename,O_RDONLY); 
if(fd == -1) {
    return -1;
}
int sum = 0;
int n2;
while(read(fd, (void *) &n2, sizeof(int)) >= 1){ 
    sum = sum + n2;
}
close(fd);
return sum;

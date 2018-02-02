int fd = open(filename, O_RDONLY); 
if(fd == -1) {
    return -1;
}
int sum = 0;
int n2 = 0;
int r = 1;
while(r >= 1){ 
    sum = sum + n2;
    r = read(fd, (void *) &n2, sizeof(int));
    if(r == -1)
        return -2;
}
if (close(fd) < 0)
    return -3;
        
return sum;


//q1
int fd = open(filename, O_RDONLY); 
if(fd == -1) {
    return -1;
}

struct stat buf;
fstat(fd, &buf);
if(index > buf.st_size){
    return -2;
}

lseek(fd, (off_t) index*sizeof(int), SEEK_SET);
unsigned int n2;
int r = read(fd, (void *) &n2, sizeof(int));
if (r == -1)
    return -1;
close(fd);
return n2;




//q2
int fd = open(filename, O_WRONLY); 
if(fd == -1) {
    return;
}
lseek(fd, (off_t) index*sizeof(unsigned int), SEEK_SET);
write(fd, (void *) &value, sizeof(unsigned int));
close(fd);
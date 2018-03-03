int fd = open(filename, O_RDONLY); 
if(fd == -1) {
    return -1;
}
close(fd);
return 0;
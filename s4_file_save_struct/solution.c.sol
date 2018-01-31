int fd = open(filename,O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR); 
if(fd == -1) 
    return -1;
for(int i = 0; i < size; i++){
    if((int)write(fd, (void *) &pt[i], sizeof(point_t)) == -1)
        return -1;
}
close(fd);
return 0;
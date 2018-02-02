int fd = open(filename,O_WRONLY|O_CREAT|O_TRUNC, 0666); 
if(fd == -1) 
    return -1;
for(int i = 0; i < size; i++){
    if((int)write(fd, (void *) &pt[i], sizeof(point_t)) == -1)
        return -2;
}
if(close(fd)<0)
    return -3;
return 0;





int fd1 = open(file_name,O_RDONLY); 
if(fd1 == -1) return -1;

struct stat st;
if (stat(file_name, &st))
    return -1;

int fd2 = open(new_file_name,O_WRONLY|O_CREAT, st.st_mode); 
if(fd2 == -1) return -1;

char c;
while(read(fd1, (void *) &c, sizeof(char)) >= 1){ 
    int w = write(fd2, (void *) &c, sizeof(char));
    if (w == -1)
        return -1;
}
if(close(fd1)<0)
    return -1;
if(close(fd2)<0)
    return -1;
return 0;




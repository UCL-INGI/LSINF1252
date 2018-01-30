int fd1 = open(file_name,O_RDONLY); 
int fd2 = open(new_file_name,O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR); 
if(fd1 == -1) return -1;
if(fd2 == -1) return -1;

char c;
while(read(fd1, (void *) &c, sizeof(char)) >= 1){ 
    int w = write(fd2, (void *) &c, sizeof(char));
    if (w == -1)
        return -1;
}
close(fd1);
close(fd2);
return 0;




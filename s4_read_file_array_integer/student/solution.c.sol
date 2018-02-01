//Open
int fd1 = open(file_name,O_RDONLY); 
int fd2 = open(new_file_name,O_WRONLY|O_CREAT); 
if(fd1 == -1) return -1;
if(fd2 == -1) return -1;

//Permissions
struct stat st;
stat(file_name, &st);
chmod(new_file_name, st.st_mode);

//Copy
char c;
while(read(fd1, (void *) &c, sizeof(char)) >= 1){ 
    if ((int)write(fd2, (void *) &c, sizeof(char)) == -1)
        return -1;
}

//Close
close(fd1);
close(fd2);
return 0;




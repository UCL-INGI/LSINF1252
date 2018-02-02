//q1
int fd = open(filename, O_RDONLY);
if (fd < 0)
    return -1;

struct stat d;
if (fstat(fd, &d) != 0)
    return -1;

if (sizeof(int)*index > d.st_size)
    return -2;

if (lseek(fd,sizeof(int)*index, SEEK_SET) != sizeof(int)*index)
    return -1;
int r;
int n = read(fd, &r,  sizeof(int));
if (n != sizeof(int))
    return -1;

close(fd);
return r;


//q2
int fd = open(filename, O_WRONLY);
lseek(fd,sizeof(int)*index, SEEK_SET);
write(fd, &value, sizeof(int));
close(fd);
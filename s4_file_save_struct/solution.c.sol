int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0666);

if (fd < 0)
    return -1;

int n = write(fd, pt, sizeof(point_t)*size);

if (n <= 0)
    return -2;

if (close(fd))
    return -3;

return 0;
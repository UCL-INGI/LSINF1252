// wrapper for the file operations, open, read, write

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "wrap.h"

int __real_open(const char *pathname, int flags, mode_t mode);
int __real_creat(const char *pathname, mode_t mode);
int __real_close(int fd);
ssize_t __real_read(int fd, void *buf, size_t count);
ssize_t __real_write(int fd, const void *buf, size_t count);
int __real_stat(const char* path, struct stat *buf);
int __real_fstat(int fd, struct stat *buf);
int __real_lstat(const char* path, struct stat *buf);
ssize_t __real_pread(int fd, void *buf, size_t count, off_t offset);
ssize_t __real_pwrite(int fd, const void *buf, size_t count, off_t offset);
off_t __real_lseek(int fd, off_t offset, int whence);

extern bool wrap_monitoring;
extern struct wrap_stats_t stats;
extern struct wrap_monitor_t monitored;
extern struct wrap_fail_t failures;
extern struct wrap_log_t logs;

int __wrap_open(char *pathname, int flags, mode_t mode) {

  if(!wrap_monitoring || !monitored.open) {
    return __real_open(pathname,flags,mode); 
  }
  stats.open.called++;
  stats.open.last_params.pathname=pathname;
  stats.open.last_params.flags=flags;
  stats.open.last_params.mode=mode;
  
  if (FAIL(failures.open)) {
    failures.open=NEXT(failures.open);
    errno=failures.open_errno;
    stats.open.last_return=failures.open_ret;
    return failures.open_ret;
  }
  failures.open=NEXT(failures.open);
  // did not fail
  int ret=__real_open(pathname, flags, mode);
  stats.open.last_return=ret;
  return ret;

}

int __wrap_creat(char *pathname, mode_t mode) {


  if(!wrap_monitoring || !monitored.creat) {
    return __real_creat(pathname,mode); 
  }
  stats.creat.called++;
  stats.creat.last_params.pathname=pathname;
  stats.creat.last_params.mode=mode;
  
  if (FAIL(failures.creat)) {
    failures.creat=NEXT(failures.creat);
    errno=failures.creat_errno;
    stats.creat.last_return=failures.creat_ret;
    return failures.creat_ret;
  }
  failures.creat=NEXT(failures.creat);
  // did not fail
  int ret=__real_creat(pathname, mode);
  stats.creat.last_return=ret;
  return ret;

}

int __wrap_close(int fd){

  if(!wrap_monitoring || !monitored.close) {
    return __real_close(fd); 
  }
  stats.close.called++;
  stats.close.last_params.fd=fd;
  
  if (FAIL(failures.close)) {
    failures.close=NEXT(failures.close);
    errno=failures.close_errno;
    stats.close.last_return=failures.close_ret;
    return failures.close_ret;
  }
  failures.close=NEXT(failures.close);
  // did not fail
  int ret=__real_close(fd);
  stats.close.last_return=ret;
  return ret;

}

int __wrap_read(int fd, void *buf, size_t count){

  if(!wrap_monitoring || !monitored.read) {
    return __real_read(fd,buf,count); 
  }
  stats.read.called++;
  stats.read.last_params.fd=fd;
  stats.read.last_params.buf=buf;
  stats.read.last_params.count=count;
  
  if (FAIL(failures.read)) {
    failures.read=NEXT(failures.read);
    errno=failures.read_errno;
    stats.read.last_return=failures.read_ret;
    return failures.read_ret;
  }
  failures.read=NEXT(failures.read);
  // did not fail
  int ret=__real_read(fd,buf,count);
  stats.read.last_return=ret;
  return ret;

}


int __wrap_write(int fd, void *buf, size_t count){

  if(!wrap_monitoring || !monitored.write) {
    return __real_write(fd,buf,count); 
  }
  stats.write.called++;
  stats.write.last_params.fd=fd;
  stats.write.last_params.buf=buf;
  stats.write.last_params.count=count;
  
  if (FAIL(failures.write)) {
    failures.write=NEXT(failures.write);
    errno=failures.write_errno;
    stats.write.last_return=failures.write_ret;
    return failures.write_ret;
  }
  failures.write=NEXT(failures.write);
  // did not fail
  int ret=__real_write(fd,buf,count);
  stats.write.last_return=ret;
  return ret;

}

int __wrap_stat(char *path, struct stat *buf) {
  
  if(!wrap_monitoring || !monitored.stat) {
return __real_stat(path,buf); 
  }
  stats.stat.called++;
  stats.stat.last_params.path=path;
  stats.stat.last_params.buf=buf;
  
  if (FAIL(failures.stat)) {
    failures.stat=NEXT(failures.stat);
    errno=failures.stat_errno;
    stats.stat.last_return=failures.stat_ret;
    return failures.stat_ret;
  }
  failures.stat=NEXT(failures.stat);
  // did not fail
  int ret=__real_stat(path,buf);
  stats.stat.returned_stat.st_dev=buf->st_dev;
  stats.stat.returned_stat.st_ino=buf->st_ino;
  stats.stat.returned_stat.st_mode=buf->st_mode;
  stats.stat.returned_stat.st_nlink=buf->st_nlink;
  stats.stat.returned_stat.st_uid=buf->st_uid;
  stats.stat.returned_stat.st_gid=buf->st_gid;
  stats.stat.returned_stat.st_rdev=buf->st_rdev;
  stats.stat.returned_stat.st_size=buf->st_size;
  stats.stat.returned_stat.st_blksize=buf->st_blksize;
  stats.stat.returned_stat.st_blocks=buf->st_blocks;
  stats.stat.returned_stat.st_atime=buf->st_atime;
  stats.stat.returned_stat.st_mtime=buf->st_mtime;
  stats.stat.returned_stat.st_ctime=buf->st_ctime;
  stats.stat.last_return=ret;
  return ret;

}

int __wrap_fstat(int fd, struct stat *buf) {

  if(!wrap_monitoring || !monitored.fstat) {
    return __real_fstat(fd,buf);
  }
  stats.fstat.called++;
  stats.fstat.last_params.fd=fd;
  stats.fstat.last_params.buf=buf;
  
  if (FAIL(failures.fstat)) {
    failures.fstat=NEXT(failures.fstat);
    errno=failures.fstat_errno;
    return failures.fstat_ret;
  }
  failures.fstat=NEXT(failures.fstat);
  // did not fail
  int ret=__real_fstat(fd,buf);
  stats.fstat.returned_stat.st_dev=buf->st_dev;
  stats.fstat.returned_stat.st_ino=buf->st_ino;
  stats.fstat.returned_stat.st_mode=buf->st_mode;
  stats.fstat.returned_stat.st_nlink=buf->st_nlink;
  stats.fstat.returned_stat.st_uid=buf->st_uid;
  stats.fstat.returned_stat.st_gid=buf->st_gid;
  stats.fstat.returned_stat.st_rdev=buf->st_rdev;
  stats.fstat.returned_stat.st_size=buf->st_size;
  stats.fstat.returned_stat.st_blksize=buf->st_blksize;
  stats.fstat.returned_stat.st_blocks=buf->st_blocks;
  stats.fstat.returned_stat.st_atime=buf->st_atime;
  stats.fstat.returned_stat.st_mtime=buf->st_mtime;
  stats.fstat.returned_stat.st_ctime=buf->st_ctime;

  stats.fstat.last_return=ret;
  return ret;

}

off_t __wrap_lseek(int fd, off_t offset, int whence) {
  
  if(!wrap_monitoring || !monitored.lseek) {
    return __real_lseek(fd,offset,whence);
  }
  stats.lseek.called++;
  stats.lseek.last_params.fd=fd;
  stats.lseek.last_params.offset=offset;
  stats.lseek.last_params.whence=whence;

  if (FAIL(failures.lseek)) {
    failures.lseek=NEXT(failures.lseek);
    errno=failures.lseek_errno;
    return failures.lseek_ret;
  }
  failures.lseek=NEXT(failures.lseek);
  // did not fail
  off_t ret=__real_lseek(fd,offset,whence);
  stats.lseek.last_return=ret;
  return ret;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

// basic structure to record the parameters of the last open call


struct params_open_t {
  char *pathname;
  int flags;
  mode_t mode;
}; 

// basic statistics for the utilisation of the open system call

struct stats_open_t {
  int called;  // number of times the open system call has been issued
  struct params_open_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last open call issued
};


struct params_creat_t {
  char *pathname;
  mode_t mode;
}; 

// basic statistics for the utilisation of the creat system call

struct stats_creat_t {
  int called;  // number of times the open system call has been issued
  struct params_creat_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last open call issued
};


struct params_close_t {
  int fd;
}; 

// basic statistics for the utilisation of the close system call

struct stats_close_t {
  int called;  // number of times the open system call has been issued
  struct params_close_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last open call issued
};

struct params_read_t {
  int fd;
  void *buf;
  ssize_t count;
}; 

// basic statistics for the utilisation of the read system call

struct stats_read_t {
  int called;  // number of times the read system call has been issued
  struct params_read_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last read call issued
};

struct params_write_t {
  int fd;
  void *buf;
  ssize_t count;
}; 

// basic statistics for the utilisation of the write system call

struct stats_write_t {
  int called;  // number of times the write system call has been issued
  struct params_read_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last read call issued
};

struct params_stat_t {
  char *path;
  struct stat *buf;
}; 

// basic statistics for the utilisation of the stat system call

struct stats_stat_t {
  int called;  // number of times the write system call has been issued
  struct params_stat_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last read call issued
  struct stat returned_stat; // last returned stat structure
};


struct params_fstat_t {
  int fd;
  struct stat *buf;
}; 

// basic statistics for the utilisation of the fstat system call

struct stats_fstat_t {
  int called;  // number of times the write system call has been issued
  struct params_fstat_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last read call issued
  struct stat returned_stat; // last returned stat structure
};


struct params_lseek_t {
  int fd;
  off_t offset;
  int whence;
}; 

// basic statistics for the utilisation of the fstat system call

struct stats_lseek_t {
  int called;  // number of times the lseek system call has been issued
  struct params_lseek_t last_params; // parameters for the last call issued
  int last_return;   // return value of the last lseek call issued
};

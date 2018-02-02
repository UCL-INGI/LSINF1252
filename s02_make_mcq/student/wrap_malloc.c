/* 
 * Wrapper for malloc, free and calloc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>

#include  "wrap.h"

#include <libintl.h> 
#include <locale.h> 
#define _(STRING) gettext(STRING)

#define MSG_SIZE 1000
char msg[MSG_SIZE];


void * __real_malloc(size_t s);
void * __real_calloc(size_t nmemb, size_t s);
void __real_free(void *);
void * __real_realloc(void *ptr, size_t size);


extern bool wrap_monitoring;
extern struct wrap_stats_t stats;
extern struct wrap_monitor_t monitored;
extern struct wrap_fail_t failures;
extern struct wrap_log_t logs;

//
// keeps only MAX_LOG in memory
//
void log_malloc(void *ptr, size_t size) {
  
  if(ptr!=NULL && logs.malloc.n < MAX_LOG) {
    logs.malloc.log[logs.malloc.n].size=size;
    logs.malloc.log[logs.malloc.n].ptr=ptr;
    logs.malloc.n++;
  }
}

void update_realloc_block(void *ptr, size_t newsize) {
   for(int i=0;i<MAX_LOG;i++) {
     if(logs.malloc.log[i].ptr==ptr) {
      logs.malloc.log[i].size=newsize;
      return;
     } 
  }
   return ;
}

size_t find_size_malloc(void *ptr) {
  for(int i=0;i<MAX_LOG;i++) {
    if(logs.malloc.log[i].ptr==ptr) 
      return logs.malloc.log[i].size;
  }
  return -1;
}


void * __wrap_malloc(size_t size) {
  if(!wrap_monitoring || !monitored.malloc) {
    return __real_malloc(size);
  }
  stats.malloc.called++;
  stats.malloc.last_params.size=size;
  if(FAIL(failures.malloc)) {
    failures.malloc=NEXT(failures.malloc);
    return failures.malloc_ret;
  }
  stats.memory.used+=size;
  failures.malloc=NEXT(failures.malloc);    
  void *ptr=__real_malloc(size);
  stats.malloc.last_return=ptr;
  log_malloc(ptr,size);
  return ptr;
}

void * __wrap_realloc(void *ptr, size_t size) {
  if(!wrap_monitoring || !monitored.realloc) {
    return __real_realloc(ptr, size);
  }
  stats.realloc.called++;
  stats.realloc.last_params.size=size;
  if(FAIL(failures.realloc)) {
    failures.realloc=NEXT(failures.realloc);
    return failures.realloc_ret;
  }
  failures.realloc=NEXT(failures.realloc);    
  int old_size=find_size_malloc(ptr);
  void *r_ptr=__real_realloc(ptr,size);
  stats.realloc.last_return=r_ptr;
  if(ptr!=NULL) {
      stats.memory.used+=size-old_size;
      update_realloc_block(ptr,size);
  }
  return r_ptr;
}


void * __wrap_calloc(size_t nmemb, size_t size) {
  if(!wrap_monitoring || !monitored.calloc) {
    return __real_calloc(nmemb, size);
  }
  stats.calloc.called++;
  stats.calloc.last_params.size=size;
  stats.calloc.last_params.nmemb=nmemb;

  if(FAIL(failures.calloc)) {
    failures.calloc=NEXT(failures.calloc);
    return failures.calloc_ret;
  }
  stats.memory.used+=nmemb*size;
  failures.calloc=NEXT(failures.calloc);
    
  void *ptr=__real_calloc(nmemb,size);
  stats.calloc.last_return=ptr;
  log_malloc(ptr,nmemb*size);
  return ptr;
}

int malloc_free_ptr(void *ptr) {

  for(int i=0;i<MAX_LOG;i++) {
    if(logs.malloc.log[i].ptr==ptr) {
      int size=logs.malloc.log[i].size;
      logs.malloc.log[i].size=-1;
      logs.malloc.log[i].ptr=NULL;
      return size;
    }
  }
  return 0;
}

void __wrap_free(void *ptr) {
  if(!wrap_monitoring || !monitored.free) {
    return __real_free(ptr);
  }
  stats.free.called++;
  stats.free.last_params.ptr=ptr;
  if(ptr!=NULL) {
      stats.memory.used-=malloc_free_ptr(ptr);
    __real_free(ptr);
  }
}


/*
void * find_ptr_malloc(size_t size){
  for(int i=0;i<MAX_LOG;i++) {
    if(logs.malloc.log[i].size==size) 
      return logs.malloc.log[i].ptr;
  }
  return NULL;
}

void malloc_log_init(struct malloc_t *l) {
  for(int i=0;i<MAX_LOG;i++) {
    l->log[i].size=-1;
    l->log[i].ptr=NULL;
  }
  l->n=0;

}

*/
int  malloc_allocated() {
  int tot=0;
  for(int i=0;i<MAX_LOG;i++) {
    if(logs.malloc.log[i].ptr!=NULL) {
      tot+=(int) logs.malloc.log[i].size;
    }
  }
  return tot;
}

/*
 * returns true if the address has been managed by malloc, false
 * otherwise (also false if address has been freed)
 */
int malloced(void *addr) {
  for(int i=0;i<MAX_LOG;i++) {
    if(logs.malloc.log[i].ptr<=addr && 
       (logs.malloc.log[i].ptr+ logs.malloc.log[i].size)>=addr) {
      return true;
    }
  }
  return false;

}

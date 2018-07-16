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
// log for malloc operations


struct malloc_elem_t {
  size_t size;
  void *ptr;
};


// basic structure to record the parameters of the last malloc call

struct params_malloc_t {
       size_t size;           
}; 

// basic statistics for the utilisation of the malloc call

struct stats_malloc_t {
  int called;  // number of times the malloc call has been issued
  struct params_malloc_t last_params; // parameters for the last call issued
  void *last_return;   // return value of the last malloc call issued
};

// basic structure to record the parameters of the last malloc call

struct params_calloc_t {
       size_t nmemb;
       size_t size;           
}; 

// basic statistics for the utilisation of the calloc call

struct stats_calloc_t {
  int called;  // number of times the malloc call has been issued
  struct params_calloc_t last_params; // parameters for the last call issued
  void *last_return;   // return value of the last malloc call issued
};


// basic structure to record the parameters of the last free call

struct params_free_t {
   void *ptr;
}; 

// basic statistics for the utilisation of the free call

struct stats_free_t {
  int called;  // number of times the free call has been issued
  struct params_free_t last_params; // parameters for the last call issued
};


// basic structure to record the parameters of the last realloc call

struct params_realloc_t {
  void *ptr;
  size_t size;           
}; 

// basic statistics for the utilisation of the realloc call

struct stats_realloc_t {
  int called;  // number of times the malloc call has been issued
  struct params_realloc_t last_params; // parameters for the last call issued
  void  *last_return;   // return value of the last realloc call issued
};


// function prototypes

//void malloc_log_init(struct malloc_t *l);
void malloc_log(void *ptr, size_t size);

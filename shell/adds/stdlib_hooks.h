#ifndef STDLIB_HOOKS_H_
#define STDLIB_HOOKS_H_

#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <malloc.h>

void hook_alloc_force_fail(void);
void hook_alloc_stop_fail(void);

/*
 * Compiler extensions are used here to make writing those definitions less
 * tedious.
 */
#define HookDeclare(function)                                \
  extern typeof(&function) hook_real_##function;             \
  extern typeof(&function) hook_current_##function;

HookDeclare(malloc);
HookDeclare(realloc);
#ifdef _GNU_SOURCE
HookDeclare(aligned_alloc);
#endif
HookDeclare(shmat);
HookDeclare(mmap);
HookDeclare(exit);

#endif

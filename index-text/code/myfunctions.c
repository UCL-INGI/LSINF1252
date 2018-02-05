#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>

/*
malloc est une copie de la fonction de la librairie standard, avec pour seule difference que lorsque la variable globale
malloc_fail est !=0, malloc renvoie NULL; on peut ainsi tester si l'etudiant gere bien le cas ou malloc ne peut pas allouer de 
la memoire.
*/
void *malloc(size_t size) 
{	
	extern int malloc_fail;
    if(malloc_fail)
	{
        return NULL;
	}
    void *(*original_malloc) (size_t size);
    original_malloc = dlsym(RTLD_NEXT, "malloc");

    void *ptr = original_malloc(size);

    return ptr;
}

/* free est une copie de la fonction de la librairie standard, avec pour seule difference qu'elle garde en memoire, via la variable 
globale free_count, le nombre de fois que free est appellee, ainsi sait si l'etudiant libere bien la memoire une fois qu'elle n'est
plus utilisee*/
void free(void *ptr)
{	
	extern int free_count;
	free_count++;
	
    void (*original_free) (void *ptr);
    original_free = dlsym(RTLD_NEXT, "free");

    original_free(ptr);
}

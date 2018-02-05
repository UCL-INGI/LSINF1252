#include<stdio.h>
#include<stdlib.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <semaphore.h>
#include "sbuf.h"

/* @pre sp!=NULL
* @post ajoute item à la fin du buffer partagé. Ce buffer est géré
*       comme une queue FIFO
*/
void sbuf_insert(sbuf_t *sp, int item)
{
	@@mp@@
}

/* @pre sbuf!=NULL
* @post retire le dernier item du buffer partagé
*/
int sbuf_remove(sbuf_t *sp)
{
	@@mp_remove@@
}

void montest(void) {

@@montest@@

}

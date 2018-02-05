
#ifndef _SWAP_H
#define _SWAP_H

struct fract_t {
        int num;
        int denum;
};

void swap(struct fract_t *a, struct fract_t *b);

#endif
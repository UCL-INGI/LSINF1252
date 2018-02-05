#ifndef _STUDENT_CODE_H_
#define _STUDENT_CODE_H_

typedef struct indexEntry {
    char word[26];
    int count; //nombre de fois qu'un mot est apparu dans le corpus
    struct indexEntry *next;
} Entry;

Entry *build_index(char *corpus);

void filter_index(Entry **index_head, int treshold);

#endif

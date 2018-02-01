typedef struct node{
    char val;
    struct node* next;
} node_t;

int compare(char a, char b);
int insert(node_t** head, char val, int (*fun)(char,char));

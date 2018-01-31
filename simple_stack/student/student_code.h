struct node{
    struct node *next;
    int value;
};

//struct node *pop(struct node *head);
int push(struct node **head, int value);

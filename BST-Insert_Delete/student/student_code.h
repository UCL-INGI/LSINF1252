typedef struct node {
    char* word;
    char* definition;
    struct node* left;
    struct node* right;
} node_t;

typedef struct bt {
    struct node* root;
} bt_t;

int insert(bt_t* bt, char* word, char* definition);

node_t* rightSubtreesLeftMostChild(node_t* node);

void delete(bt_t* bt, char* word);

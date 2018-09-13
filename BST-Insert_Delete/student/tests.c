#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "student_code.h"
#include "CTester/CTester.h"

/*
 * @return: number of times @value is contained in @array
 */
int containsArray(int* array, int size, int value){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(array[i] == value)
            count++;
    }
    return count;
}

int sameNodes(node_t* node1, node_t* node2){
    if(node1 == NULL && node2 == NULL)
        return true;
    if(node1 == NULL){
        push_info_msg(_("One node in your solution tree should be NULL"));
        return false;
    }
    if(node2 == NULL){
        push_info_msg(_("One node in your solution is NULL and shouldn't be"));
        return false;
    }
    if(node2->enWord == NULL || node2->frWord == NULL){
        push_info_msg(_("One of the 'enWord' and 'frWord' attributes is NULL in one of your nodes"));
        return false;
    }
    if(strcmp(node1->enWord, node2->enWord) || strcmp(node1->frWord, node2->frWord)){
        if(!strcmp(node2->enWord,"changed")){
            push_info_msg(_("You certainly did node->enWord = enWord. This doesn't copy the string. So if enWord gets changed in the test function, the word in your tree will also change ! Hint : use strcpy"));
        }
        else if(!strcmp(node2->frWord,"changed")){
            push_info_msg(_("You certainly did node->frWord = frWord. This doesn't copy the string. So if enWord gets changed in the test function, the word in your tree will also change ! Hint : use strcpy"));
        }
        else{
            char error_msg[100];
            sprintf(error_msg, "One (of your) wrong node(s) : enWord : %s, frWord : %s. Solution : enWord : %s, frWord : %s.", node2->enWord, node2->frWord, node1->enWord, node1->frWord);
            push_info_msg(_(error_msg));
            return false;
        }
    }
    return sameNodes(node1->left, node2->left) && sameNodes(node1->right, node2->right);
}

int sameTrees(bt_t* solT, bt_t* tree){
    if(solT == NULL && tree == NULL)
        return true;
    //never happens, we don't have NULL trees
    if(solT == NULL || tree == NULL)
        return false;
    return sameNodes(solT->root, tree->root);
}

void freeNode(node_t* node){
    if(!node)
        return;
    if(node->enWord)
        free(node->enWord);
    if(node->frWord)
        free(node->frWord);
    if(node->left)
        freeNode(node->left);
    if(node->right)
        freeNode(node->right);
    free(node);
}

node_t* init_node(char* enWord, char* frWord){
    node_t* newnode = malloc(sizeof(node_t));
    if(!newnode)
        return NULL;
    newnode->enWord = (char*) malloc(sizeof(char)*(strlen(enWord)+1));
    if(!(newnode->enWord)){
        free(newnode);
        return NULL;
    }
    strcpy(newnode->enWord,enWord);
    newnode->frWord = (char*) malloc(sizeof(char)*(strlen(frWord)+1));
    if(!(newnode->frWord)){
        free(newnode->enWord);
        free(newnode);
        return NULL;
    }
    strcpy(newnode->frWord,frWord);
    newnode->left = newnode->right = NULL;
    return newnode;
}

void freeBt(bt_t* bt){
    if(!bt)
        return;
    if(bt->root)
        freeNode(bt->root);
}

bt_t* init_bt(char* enWord, char* frWord){
    node_t* root = init_node(enWord,frWord);
    if(!root)
        return NULL;
    bt_t* bt = (bt_t*) malloc(sizeof(bt_t));
    if(!bt){
        freeNode(bt->root);
        return NULL;
    }
    bt->root = root;
    return bt;
}

bt_t* tree1(){
    bt_t* tree = init_bt("cat","chat");
    if(!tree){
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    node_t* node1 = init_node("animal","animal");
    node_t* node2 = init_node("deer","biche");
    node_t* node3 = init_node("creature","créature");
    node_t* node4 = init_node("elephant","éléphant");
    node_t* node5 = init_node("dog","chien");
    node_t* node6 = init_node("eagle","aigle");
    node_t* node7 = init_node("sponge","éponge");

    if(!node1 || !node2 || !node3 || !node4 || !node5 || !node6 || !node7){
        freeNode(tree->root);
        if(node1) freeNode(node1);
        if(node2) freeNode(node2);
        if(node3) freeNode(node3);
        if(node4) freeNode(node4);
        if(node5) freeNode(node5);
        if(node6) freeNode(node6);
        if(node7) freeNode(node7);
        free(tree);
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    else{
        (tree->root)->left = node1; (tree->root)->right = node2;
        node2->left = node3; node2->right = node4;
        node4->left = node5; node4->right = node7;
        node5->right = node6;
    }
    return tree;
}

bt_t* tree2(){
    bt_t* tree = init_bt("cat","chat");
    if(!tree){
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    node_t* node1 = init_node("animal","animal");
    node_t* node2 = init_node("deer","biche");
    node_t* node3 = init_node("creature","créature");
    node_t* node4 = init_node("horse","cheval");
    node_t* node5 = init_node("dog","chien");
    node_t* node6 = init_node("eagle","aigle");
    node_t* node7 = init_node("sponge","éponge");
    node_t* node8 = init_node("dodo","dodo");
    node_t* node9 = init_node("fish","poisson");
    node_t* node10 = init_node("falcon","faucon");

    if(!node1 || !node2 || !node3 || !node4 || !node5 || !node6 || !node7 || !node8 ||!node9){
        freeNode(tree->root);
        if(node1) freeNode(node1);
        if(node2) freeNode(node2);
        if(node3) freeNode(node3);
        if(node4) freeNode(node4);
        if(node5) freeNode(node5);
        if(node6) freeNode(node6);
        if(node7) freeNode(node7);
        if(node8) freeNode(node8);
        if(node9) freeNode(node9);
        free(tree);
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    else{
        // = NULL done in init_node
        (tree->root)->left = node1; (tree->root)->right = node2;
        node2->left = node3; node2->right = node4;
        node4->left = node5; node4->right = node7;
        node5->left = node8; node5->right = node6;
        node6->right = node9;
        node9->left = node10;
    }
    return tree;
}

bt_t* tree3(){
    bt_t* tree = init_bt("cat","chat");
    if(!tree){
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    node_t* node1 = init_node("animal","animal");

    if(!node1){
        freeNode(tree->root);
        if(node1) freeNode(node1);
        free(tree);
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    else{
        // = NULL done in init_node
        (tree->root)->left = node1;
    }
    return tree;
}

bt_t* tree4(){
    bt_t* tree = init_bt("cat","chat");
    if(!tree){
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    node_t* node1 = init_node("animal","animal");
    node_t* node2 = init_node("deer","biche");
    node_t* node3 = init_node("creature","créature");
    node_t* node4 = init_node("elephant","éléphant");
    node_t* node7 = init_node("sponge","éponge");

    if(!node1 || !node2 || !node3 || !node4 || !node7){
        freeNode(tree->root);
        if(node1) freeNode(node1);
        if(node2) freeNode(node2);
        if(node3) freeNode(node3);
        if(node4) freeNode(node4);
        if(node7) freeNode(node7);
        free(tree);
        CU_FAIL(_("Internal error while allocating memory"));
        return NULL;
    }
    else{
        (tree->root)->left = node1; (tree->root)->right = node2;
        node2->left = node3; node2->right = node4;
        node4->right = node7;
    }
    return tree;
}

void test_insert_normal(){
    set_test_metadata("insert", _("Test in a normal case"), 1);
    bt_t* tree = tree1();
    char* newEnWord = malloc(sizeof(char)*5);
    char* newFrWord = malloc(sizeof(char)*5);


    bt_t* solT = tree1();
    node_t* dodonode = init_node("dodo","dodo");

    if(!dodonode || !newEnWord || !newFrWord || !tree || !solT){
        if(dodonode)
            freeNode(dodonode);
        if(newEnWord)
            free(newEnWord);
        if(newFrWord)
            free(newFrWord);
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }
    //TODO Hard coded feature ? better to compare with a real tree ?
    //It takes more time but we can be sure of the answer with that...
    //'dodo' place was hardcoded, should we use our own function to insert it ?
    ((((solT->root)->right)->right)->left)->left = dodonode;

    strcpy(newEnWord, "dodo");
    strcpy(newFrWord, "dodo");

    //to be able to know how many bytes the student malloced
    int begin = logs.malloc.n;

    monitored.malloc = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    //to be sure he copies the string
    newEnWord = realloc(newEnWord, sizeof(char)*8);
    newFrWord = realloc(newFrWord, sizeof(char)*8);
    strcpy(newEnWord, "changed");
    strcpy(newFrWord, "changed");
    //TODOFreeIfNewEnWordNULL

    // check if only 3 malloc (enWord, frWord, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 3);
    if(nbMalloc != 3)
        push_info_msg(_("You can only use 3 calls to malloc for this case"));
    else{ // did he allocate enough memory ? or not too much ?
        int size[3];
        size[0] = logs.malloc.log[begin].size;
        size[1] = logs.malloc.log[begin+1].size;
        size[2] = logs.malloc.log[begin+2].size;
        if(!containsArray(size,3,sizeof(node))){
            CU_FAIL();
            push_info_msg(_("You didn't malloc the right space for the node. You should have malloced 32 bytes since you have 4 pointers (of 8 bytes each)."));
        }
        // 5 = size of the enWord and frWord given before changing ("dodo")
        if(containsArray(size,3,5) != 2){
            CU_FAIL();
            //TODO Question : why \\\\ for only one backslash ?
            push_info_msg(_("You didn't malloc the right space for the strings. Don't forget to count the '\\\\0' character !"));
        }
    }
    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    CU_ASSERT_EQUAL(inserted, 1);
    if(inserted != 1)
        push_info_msg(_("The returned value should be 1"));
}

void test_insert_normal_first_malloc_fails(){
    set_test_metadata("insert", _("Test when malloc fails"), 1);
    bt_t* tree = tree1();
    char* newEnWord = malloc(sizeof(char)*5);
    char* newFrWord = malloc(sizeof(char)*5);

    bt_t* solT = tree1();

    if(!newEnWord || !newFrWord || !tree || !solT){
        if(newEnWord)
            free(newEnWord);
        if(newFrWord)
            free(newFrWord);
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(newEnWord, "dodo");
    strcpy(newFrWord, "dodo");

    failures.malloc = FAIL_FIRST;
    monitored.malloc = true;
    monitored.free = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;
    /*
    // TODO : FIX, doesnt' work
    //to be sure he copies the string
    int enStillMalloced = malloced(newEnWord);
    int frStillMalloced = malloced(newFrWord);
    if(!enStillMalloced || !frStillMalloced){
        CU_FAIL();
        push_info_msg(_("You cannot free the arguments of the function !"));
    }
    */

    // check if only 1 malloc (enWord, frWord, node) since it fails.
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 1);
    if(nbMalloc != 1)
        push_info_msg(_("You should have used malloc only once in this case"));

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 0);
    if(nbFree != 0)
        push_info_msg(_("You shouldn't have called free"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    CU_ASSERT_EQUAL(inserted, 0);
    if(inserted != 0)
        push_info_msg(_("The returned value should be 0"));
}

void test_insert_normal_second_malloc_fails(){
    set_test_metadata("insert", _("Test when second malloc fails"), 1);
    bt_t* tree = tree1();
    char* newEnWord = malloc(sizeof(char)*5);
    char* newFrWord = malloc(sizeof(char)*5);

    bt_t* solT = tree1();

    if(!newEnWord || !newFrWord || !tree || !solT){
        if(newEnWord)
            free(newEnWord);
        if(newFrWord)
            free(newFrWord);
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(newEnWord, "dodo");
    strcpy(newFrWord, "dodo");

    failures.malloc = FAIL_SECOND;
    monitored.malloc = true;
    monitored.free = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    /*
    // TODO : FIX, doesnt' work
    //to be sure he copies the string
    int enStillMalloced = malloced(newEnWord);
    int frStillMalloced = malloced(newFrWord);
    if(!enStillMalloced || !frStillMalloced){
        CU_FAIL();
        push_info_msg(_("You cannot free the arguments of the function !"));
    }
    */

    // check if only 2 malloc (enWord, frWord, node) since the second one fails.
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 2);
    if(nbMalloc != 2)
        push_info_msg(_("You should have used malloc only twice in this case"));

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 1);
    if(nbFree != 1)
        push_info_msg(_("You should have called free once"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    CU_ASSERT_EQUAL(inserted, 0);
    if(inserted != 0)
        push_info_msg(_("The returned value should be 0"));
}

void test_insert_normal_third_malloc_fails(){
    set_test_metadata("insert", _("Test when third malloc fails"), 1);
    bt_t* tree = tree1();
    char* newEnWord = malloc(sizeof(char)*5);
    char* newFrWord = malloc(sizeof(char)*5);

    bt_t* solT = tree1();

    if(!newEnWord || !newFrWord || !tree || !solT){
        if(newEnWord)
            free(newEnWord);
        if(newFrWord)
            free(newFrWord);
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(newEnWord, "dodo");
    strcpy(newFrWord, "dodo");

    failures.malloc = FAIL_THIRD;
    monitored.malloc = true;
    monitored.free = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    /*
    // TODO : FIX, doesnt' work
    //to be sure he copies the string
    int enStillMalloced = malloced(newEnWord);
    int frStillMalloced = malloced(newFrWord);
    if(!enStillMalloced || !frStillMalloced){
        CU_FAIL();
        push_info_msg(_("You cannot free the arguments of the function !"));
    }
    */

    // check if only 3 malloc (enWord, frWord, node)
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 3);
    if(nbMalloc != 3)
        push_info_msg(_("You should have used malloc only three times in this case"));

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 2);
    if(nbFree != 2)
        push_info_msg(_("You should have called free twice"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    CU_ASSERT_EQUAL(inserted, 0);
    if(inserted != 0)
        push_info_msg(_("The returned value should be 0"));
}

void test_insert_empty_tree(){
    set_test_metadata("insert", _("Test with an empty tree"),1);
    //student arguments
    bt_t* tree = malloc(sizeof(bt_t));
    tree->root = NULL;
    bt_t *solT = init_bt("dodo","dodo");
    char* newEnWord = malloc(sizeof(char)*5);
    char* newFrWord = malloc(sizeof(char)*5);

    //solution
    if(!newEnWord || !newFrWord || !tree || !solT){
        if(newEnWord)
            free(newEnWord);
        if(newFrWord)
            free(newFrWord);
        if(tree)
            free(tree);
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(newEnWord, "dodo");
    strcpy(newFrWord, "dodo");

    monitored.malloc = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    //to be sure he copies the string
    newEnWord = realloc(newEnWord, sizeof(char)*8);
    newFrWord = realloc(newFrWord, sizeof(char)*8);
    strcpy(newEnWord, "changed");
    strcpy(newFrWord, "changed");
    //TODOFreeIfNewEnWordNULL

    // check if only 3 malloc (enWord, frWord, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 3);
    if(nbMalloc != 3)
        push_info_msg(_("You can only use 3 calls to malloc for this case"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    CU_ASSERT_EQUAL(inserted, 1);
    if(inserted != 1)
        push_info_msg(_("The returned value should be 1"));
}

void test_insert_already_inserted(){
    set_test_metadata("insert", _("Test with an already inserted node"),1);
    //student arguments
    bt_t* tree = tree1();
    char* newEnWord = malloc(sizeof(char)*4);
    char* newFrWord = malloc(sizeof(char)*5);

    //solution
    bt_t* solT = tree1();

    if(!newEnWord || !newFrWord || !tree || !solT){
        if(newEnWord)
            free(newEnWord);
        if(newFrWord)
            free(newFrWord);
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(newEnWord, "cat");
    strcpy(newFrWord, "chat");

    monitored.malloc = true;
    monitored.free = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    // Check : 0 malloc, 0 free
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 0);
    if(nbMalloc != 0)
        push_info_msg(_("You can't use malloc in this case"));

    int nbFree = stats.malloc.called;
    CU_ASSERT_EQUAL(nbFree, 0);
    if(nbFree != 0)
        push_info_msg(_("Why did you use free ?"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    CU_ASSERT_EQUAL(inserted, 0);
    if(inserted != 0)
        push_info_msg(_("The returned value should be 0"));
}

void test_delete_no_child(){
    set_test_metadata("delete", _("Test with a node having no child"),1);
    //student arguments
    bt_t* tree = tree1();
    char *enWord = "eagle";

    bt_t* solT = tree1();

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    freeNode(((((solT->root)->right)->right)->left)->right);
    ((((solT->root)->right)->right)->left)->right = NULL;

    /*
    //TODO should do ? harder when there is a child... we could delete 1 or another node...
    //to be sure he free's the node after the key and value
    void *ptr = (void*) (((((tree->root)->right)->right)->left)->right);
    */

    //TODO monitored malloc to see he isn't doing anything wrong ?
    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 3);
    if(nbFree != 3)
        push_info_msg(_("You should use free 3 times"));
    /*
    //TODO should do ? harder when there is a child... we could delete 1 or another node...
    else{
        void *last_ptr = (void*) stats.free.last_params.ptr;
        CU_ASSERT_EQUAL(ptr, last_ptr);
        if (ptr != last_ptr)
            push_info_msg(_("The last free you should do is on the struct (node)"));

    }
    */

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

}

void test_delete_one_child(){
    set_test_metadata("delete", _("Test with a node having only one child"),1);
    //student arguments
    bt_t* tree = tree1();
    char* enWord = "dog";

    //solution
    bt_t* solT = tree1();

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    node_t* elephant = ((solT->root)->right)->right;
    node_t* dog = elephant->left;
    elephant->left = dog->right;
    free(dog->enWord);
    free(dog->frWord);
    free(dog);


    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 3);
    if(nbFree != 3)
        push_info_msg(_("You should use free 3 times"));
    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
}

void test_delete_node_not_found(){
    set_test_metadata("delete", _("Test with a key that isn't in the tree"),1);
    //student arguments
    bt_t* tree = tree1();
    char* enWord = "dodo";

    //solution
    bt_t* solT = tree1();

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 0);
    if(nbFree != 0)
        push_info_msg(_("You shouldn't use free in this case"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
}

void test_delete_root_replace_null(){
    set_test_metadata("delete", _("Test deleting the root of the tree, with no child"),1);
    //student arguments
    char* enWord = "dodo";
    char* frWord = "dodo";
    bt_t* tree = init_bt(enWord, frWord);

    //solution
    bt_t* solT = malloc(sizeof(bt_t));
    solT->root = NULL;

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 3);
    if(nbFree != 3)
        push_info_msg(_("You should use free 3 times"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    if(tree){
        if(tree->root){
            CU_FAIL();
            push_info_msg(_("Tree->root should be NULL"));
        }
    }
}

void test_delete_root_replace_node(){
    set_test_metadata("delete", _("Test deleting the root of the tree, with a child"),1);
    //student arguments
    bt_t* tree = tree3();
    char* enWord = "cat";

    //solution
    bt_t* solT = init_bt("animal","animal");

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 3);
    if(nbFree != 3)
        push_info_msg(_("You should use free 3 times"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

    if(tree){
        if(strcmp((tree->root)->enWord, (solT->root)->enWord) || strcmp((tree->root)->frWord, (solT->root)->enWord)){
            CU_FAIL();
            push_info_msg(_("The root of your tree is wrong. Did you update it ?"));
        }
    }
}

void test_delete_empty_tree(){
    set_test_metadata("delete", _("Test empty tree"),1);
    //student arguments
    char* enWord = "dodo";
    bt_t* tree = malloc(sizeof(bt_t));
    tree->root = NULL;

    //solution
    bt_t* solT = malloc(sizeof(bt_t));
    solT->root = NULL;

    if(!tree || !solT){
        if(tree)
            free(tree);
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;

    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 0);
    if(nbFree != 0)
        push_info_msg(_("You shouldn't use free in this case"));

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));

}

void test_delete_two_children_tree1(){
    set_test_metadata("delete", _("Test deleting a simple node (which has 2 children) using the algorithm explained above"),1);
    //student arguments
    bt_t* tree = tree1();
    bt_t* solT = tree1();

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    //sol
    node_t* deerdog = (solT->root)->right;
    free(deerdog->enWord); free(deerdog->frWord);
    node_t* dog = (deerdog->right)->left;
    deerdog->enWord = dog->enWord;
    deerdog->frWord = dog->frWord;
    node_t* elephant = deerdog->right;
    elephant->left = dog->right; //eagle
    free(dog);

    char* deerStr= "deer";

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree, deerStr);
    SANDBOX_END;

    int nbMalloc = stats.malloc.called;
    int nbFree = stats.free.called;


    CU_ASSERT_EQUAL(nbFree,3);
    if(nbFree != 3){
        push_info_msg(_("You should use free 3 times"));
    }

    CU_ASSERT_EQUAL(nbMalloc,0);
    if(nbMalloc != 0){
        push_info_msg(_("Why are you using malloc ?"));
    }

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
}

void test_delete_two_children_tree1_root(){
    set_test_metadata("delete", _("Test deleting the root node (which has 2 children) using the algorithm explained above"),1);
    //student arguments
    bt_t* tree = tree1();
    bt_t* solT = tree1();

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    //sol
    node_t* cat = solT->root;
    node_t* deer = cat->right;
    node_t* creature = deer->left;
    solT->root = creature;
    creature->left = cat->left;
    creature->right = cat->right;
    free(cat->enWord); free(cat->frWord); free(cat);
    deer->left = NULL;


    char* catStr = "cat";

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree, catStr);
    SANDBOX_END;

    int nbMalloc = stats.malloc.called;
    int nbFree = stats.free.called;


    CU_ASSERT_EQUAL(nbFree,3);
    if(nbFree != 3){
        push_info_msg(_("You should use free 3 times"));
    }

    CU_ASSERT_EQUAL(nbMalloc,0);
    if(nbMalloc != 0){
        push_info_msg(_("Why are you using malloc ?"));
    }

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
}

void test_delete_two_children_tree4(){
    set_test_metadata("delete", _("Test deleting a simple node (which has 2 children) using the algorithm explained above. The node's right subtree's leftmost child is node->right"),1);
    //student arguments
    bt_t* tree = tree4();
    bt_t* solT = tree4();

    if(!tree || !solT){
        if(tree){
            freeNode(tree->root);
            free(tree);
        }
        if(solT){
            freeNode(solT->root);
            free(solT);
        }
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    //sol
    node_t* cat = solT->root;
    node_t* deer = cat->right;
    node_t* elephant = deer->right;
    cat->right = elephant;
    elephant->left = deer->left;
    free(deer->enWord); free(deer->frWord); free(deer);
    //sol built

    char* deerStr = "deer";

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree, deerStr);
    SANDBOX_END;

    int nbMalloc = stats.malloc.called;
    int nbFree = stats.free.called;


    CU_ASSERT_EQUAL(nbFree,3);
    if(nbFree != 3){
        push_info_msg(_("You should use free 3 times"));
    }

    CU_ASSERT_EQUAL(nbMalloc,0);
    if(nbMalloc != 0){
        push_info_msg(_("Why are you using malloc ?"));
    }

    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
}


int main(int argc,char** argv)
{
    BAN_FUNCS(strdut,calloc,mmap);
    RUN(test_insert_normal, test_insert_normal_first_malloc_fails, test_insert_normal_second_malloc_fails, test_insert_normal_third_malloc_fails, test_insert_empty_tree, test_insert_already_inserted, test_delete_no_child, test_delete_one_child, test_delete_node_not_found, test_delete_root_replace_null, test_delete_root_replace_node, test_delete_empty_tree, test_delete_two_children_tree1, test_delete_two_children_tree1_root, test_delete_two_children_tree4);
}

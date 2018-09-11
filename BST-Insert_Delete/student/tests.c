/*
 * 4/5 tests on insert (need the fail malloc + fail malloc with null tree ? il faudrait free le tree du coup ? too hard ?). Question on NULL tree test.
 * 0/5 tests on delete
 * 0/? test on right subtree's leftmost node
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "student_code.h"
#include "CTester/CTester.h"

int sameNodes(node_t* node1, node_t* node2){
    if(node1 == NULL && node2 == NULL)
        return true;
    if(node1 == NULL || node2 == NULL)
        return false;
    if(strcmp(node1->enWord, node2->enWord) || strcmp(node1->frWord, node2->frWord))
        return false;
    return sameNodes(node1->left, node2->left) && sameNodes(node1->right, node2->right);
}

int sameTrees(bt_t* compTree, bt_t* tree){
    if(compTree == NULL && tree == NULL)
        return true;
    if(compTree == NULL || tree == NULL)
        return false;
    return sameNodes(compTree->root, tree->root);
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
    if(!tree)
        CU_FAIL(_("Internal error while allocating memory"));
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
    }
    else{
        (tree->root)->left = node1; (tree->root)->right = node2;
        node2->left = node3; node2->right = node4;
        node4->left = node5; node4->right = node7;
        node5->right = node6;
    }
    return tree;
}


void test_insert_normal(){
    set_test_metadata("insert", _("Test in a normal case"), 1);
    bt_t* tree = tree1();
    char* newEnWord = "dodo";
    char* newFrWord = "dodo";

    bt_t* compTree = tree1();

    monitored.malloc = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    // check if only 3 malloc (enWord, frWord, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 3);
    if(nbMalloc != 3)
        push_info_msg(_("You can only use 3 calls to malloc for this case"));
    //TODO Hard coded feature ? better to compare with a real tree ?
    //It takes more time but we can be sure of the answer with that...
    //'dodo' place was hardcoded, should we use our own function to insert it ?
    ((((compTree->root)->right)->right)->left)->left = init_node("dodo","dodo");
    int sameT = sameTrees(compTree,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
    
    CU_ASSERT_EQUAL(inserted, 1);
    if(inserted != 1)
        push_info_msg(_("The returned value should be 1"));
}

void test_insert_empty_tree(){
    set_test_metadata("insert", _("Test with an empty tree"),1);
    //student arguments
    bt_t* tree = malloc(sizeof(bt_t));
    tree->root = NULL;
    char* newEnWord = "dodo";
    char* newFrWord = "dodo";

    //solution
    bt_t* solT = init_bt(newEnWord,newFrWord);
    
    if(!tree || !solT){
        if(tree)
            free(tree);
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        //TODO return ??? Or CU_FAIL is enough ?
    }

    monitored.malloc = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

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

void test_insert_null_tree(){
    set_test_metadata("insert", _("Test with a null tree"),1);
    //student arguments
    bt_t* tree = NULL;
    char* newEnWord = "dodo";
    char* newFrWord = "dodo";

    //solution
    bt_t* solT = init_bt(newEnWord,newFrWord);
    
    if(!tree || !solT){
        if(tree)
            free(tree);
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        //TODO return ??? Or CU_FAIL is enough ?
    }

    monitored.malloc = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    // check if only 4 malloc (enWord, frWord, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 4);
    if(nbMalloc != 4)
        push_info_msg(_("You can only use 4 calls to malloc for this case"));

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
    char* newEnWord = "cat";
    char* newFrWord = "chat";
    
    //solution
    bt_t* solT = tree1();
    
    if(!tree || !solT){
        if(tree)
            free(tree);
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        //TODO return ??? Or CU_FAIL is enough ?
    }
    
    monitored.malloc = true;
    monitored.free = true;
    int inserted;
    SANDBOX_BEGIN;
    inserted = insert(tree, newEnWord, newFrWord);
    SANDBOX_END;
    
    //TODO : on pourrait check la mémoire aussi mais s'il malloc puis free, il aura bon donc que faire?
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
    char* enWord = "eagle";
    
    bt_t* solT = tree1();
    freeNode(((((solT->root)->right)->right)->left)->right);
    ((((solT->root)->right)->right)->left)->right = NULL;
    //TODO special feedback in case he free's but doesn't put NULL instead ?
    
    //TODO monitored malloc to see he isn't doing anything wrong ?
    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;
    
    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 3);
    if(nbFree != 3)
        push_info_msg(_("Wrong number of free's"));
    
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
        push_info_msg(_("Wrong number of free's"));
    
    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
}

void test_delete_two_children(){
    // TODO
}

void test_delete_node_not_found(){
    set_test_metadata("delete", _("Test with a key that isn't in the tree"),1);
    //student arguments
    bt_t* tree = tree1();
    char* enWord = "dodo";
    
    //solution
    bt_t* solT = tree1();
    
    monitored.free = true;
    SANDBOX_BEGIN;
    delete(tree,enWord);
    SANDBOX_END;
    
    int nbFree = stats.free.called;
    CU_ASSERT_EQUAL(nbFree, 0);
    if(nbFree != 0)
        push_info_msg(_("Wrong number of free's"));
    
    int sameT = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameT,true);
    if(!sameT)
        push_info_msg(_("Your tree isn't what was expected"));
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_insert_normal, test_insert_empty_tree, test_insert_null_tree, test_insert_already_inserted, test_delete_no_child, test_delete_one_child, test_delete_node_not_found);
}

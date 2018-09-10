/*
 * 3/5 tests on insert (need the fail malloc + the insert not done because key is already present)
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
    newnode->enWord = (char*) malloc(strlen(enWord));
    if(!(newnode->enWord)){
        free(newnode);
        return NULL;
    }
    strcpy(newnode->enWord,enWord);
    newnode->frWord = (char*) malloc(strlen(frWord));
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
    bt_t* tree = init_bt("cat","a small domesticated carnivorous mammal");
    if(!tree)
        CU_FAIL(_("Internal error while allocating memory"));
    node_t* node1 = init_node("animal","animal def");
    node_t* node2 = init_node("deer","deer def");
    node_t* node3 = init_node("creature","creature def");
    node_t* node4 = init_node("elephant","elephant def");
    node_t* node5 = init_node("dog","dog def");
    node_t* node6 = init_node("eagle","eagle def");
    node_t* node7 = init_node("sponge","sponge def");

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
    char* newFrWord = "dodo def";

    bt_t* compTree = tree1();

    monitored.malloc = true;

    SANDBOX_BEGIN;
    insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    // check if only 3 malloc (enWord, frWord, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 3);
    if(nbMalloc != 3)
        push_info_msg(_("You can only use 3 calls to malloc for this case"));
    //TODO Hard coded feature ? better to compare with a real tree ?
    //It takes more time but we can be sure of the answer with that...
    //'dodo' place was hardcoded, should we use our own function to insert it ?
    ((((compTree->root)->right)->right)->left)->left = init_node("dodo","dodo def");
    int sameTrees = sameTrees(compTree,tree);
    CU_ASSERT_EQUAL(sameTrees,true);
    if(!sameTrees)
        push_info_msg(_("Your tree isn't what was expected"));
}

void test_insert_empty_tree(){
    set_test_metadata("insert", _("Test with an empty tree"),1);
    //student arguments
    bt_t* tree = malloc(sizeof(bt_t));
    tree->root = NULL;
    char* newEnWord = "dodo";
    char* newFrWord = "dodo def";

    //solution
    bt_t* solT = malloc(sizeof(bt_t));
    node_t* node = init_node(newEnWord,newFrWord);
    solT->root = node;
    if(!tree || !solT || !node){
        if(node)
            free(node);
        if(tree)
            free(tree);
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        //TODO return ??? Or CU_FAIL is enough ?
    }

    monitored.malloc = true;

    SANDBOX_BEGIN;
    insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    // check if only 3 malloc (enWord, frWord, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 3);
    if(nbMalloc != 3)
        push_info_msg(_("You can only use 3 calls to malloc for this case"));

    int sameTrees = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameTrees,true);
    if(!sameTrees)
        push_info_msg(_("Your tree isn't what was expected"));
}

void test_insert_null_tree(){
    set_test_metadata("insert", _("Test with a null tree"),1);
    //student arguments
    bt_t* tree = NULL;
    char* newEnWord = "dodo";
    char* newFrWord = "dodo def";

    //solution
    bt_t* solT = malloc(sizeof(bt_t));
    node_t* node = init_node(newEnWord,newFrWord);
    solT->root = node;

    if(!tree || !solT || !node){
        if(node)
            free(node);
        if(tree)
            free(tree);
        if(solT)
            free(solT);
        CU_FAIL(_("Internal error while allocating memory"));
        //TODO return ??? Or CU_FAIL is enough ?
    }

    monitored.malloc = true;

    SANDBOX_BEGIN;
    insert(tree, newEnWord, newFrWord);
    SANDBOX_END;

    // check if only 4 malloc (enWord, frWord, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 4);
    if(nbMalloc != 4)
        push_info_msg(_("You can only use 4 calls to malloc for this case"));

    int sameTrees = sameTrees(solT,tree);
    CU_ASSERT_EQUAL(sameTrees,true);
    if(!sameTrees)
        push_info_msg(_("Your tree isn't what was expected"));
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_insert_normal, test_insert_empty_tree, test_insert_null_tree);
}

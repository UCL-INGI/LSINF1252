// CTester template

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
    if(strcmp(node1->word, node2->word) || strcmp(node1->definition, node2->definition))
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
    if(node->word)
        free(node->word);
    if(node->definition)
        free(node->definition);
    if(node->left)
        freeNode(node->left);
    if(node->right)
        freeNode(node->right);
}

node_t* init_node(char* word, char* definition){
    node_t* newnode = malloc(sizeof(node_t));
    if(!newnode)
        return NULL;
    newnode->word = (char*) malloc(strlen(word));
    if(!(newnode->word)){
        free(newnode);
        return NULL;
    }
    strcpy(newnode->word,word);
    newnode->definition = (char*) malloc(strlen(definition));
    if(!(newnode->definition)){
        free(newnode->word);
        free(newnode);
        return NULL;
    }
    strcpy(newnode->definition,definition);
    newnode->left = newnode->right = NULL;
    return newnode;
}

void freeBt(bt_t* bt){
    if(!bt)
        return;
    if(bt->root)
        freeNode(bt->root);
}

bt_t* init_bt(char* word, char* definition){
    node_t* root = init_node(word,definition);
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
    node_t* node2 = init_node("dog","dog def");
    node_t* node3 = init_node("creature","creature def");
    node_t* node4 = init_node("elephant","elephant def");
    node_t* node5 = init_node("deer","deer def");
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


void test_insert_normal() {
    push_info_msg(_("test"));
    set_test_metadata("insert", _("Test in a normal case"), 1);
    bt_t* tree = tree1();
    char* newword = "dodo";
    char* newdef = "dodo def";

    bt_t* compTree = tree1();

    monitored.malloc = true;

    SANDBOX_BEGIN;
    insert(tree, newword, newdef);
    SANDBOX_END;
    
    // check if only 3 malloc (word, definition, node);
    int nbMalloc = stats.malloc.called;
    CU_ASSERT_EQUAL(nbMalloc, 3);
    if(nbMalloc != 3)
        push_info_msg(_("You can only use 3 calls to malloc for this case"));
    //TODO Hard coded feature ? better to compare with a real tree ?
    //It takes more time but we can be sure of the answer with that...
    //'dodo' place was hardcoded, should we use our own function to insert it ?
    ((((compTree->root)->right)->right)->left)->left = init_node("dodo","dodo def");
    CU_ASSERT_EQUAL(sameTrees(compTree,tree),true);
    if(!sameTrees(compTree,tree))
        push_info_msg(_("Your tree isn't what was expected"));
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_insert_normal);
}

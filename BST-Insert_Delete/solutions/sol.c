#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

/*
 * TODO
 */
int insert(bt_t* bt, char* enWord, char*frWord){
    node_t* runner = bt->root;
    node_t* parent = NULL;
    int lequel = -1;
    while(runner != NULL){
        int cmp = strcmp(enWord, runner->enWord);
        parent = runner;
        if(cmp == 0){
            return 0;
        }
        else if(cmp < 0){
            runner = runner->left;
            lequel = 1;
        }
        else{
            runner = runner->right;
            lequel = 2;
        }
    }

    node_t* t = (node_t*)malloc(sizeof(node_t));
    if(t == NULL){
        return 0;
    }
    char* n_e = (char*)malloc(sizeof(char)*(strlen(enWord)+1));
    if(n_e == NULL){
        free(t);
        return 0;
    }
    strcpy(n_e, enWord);

    char* n_f = (char*)malloc(sizeof(char)*(strlen(frWord)+1));
    if(n_f == NULL){
        free(t);
        free(n_e);
        return 0;
    }
    strcpy(n_f, frWord);

    t->enWord = n_e;
    t->frWord = n_f;
    t->left = NULL;
    t->right = NULL;

    if(parent == NULL){
        bt->root = t;
        return 1;
    }
    if(lequel == 1){
        parent->left = t;
        return 1;
    }
    else{
        parent->right = t;
        return 1;
    }
}

void delete(bt_t* bt, char* enWord){
    if(bt == NULL){
        return;
    }

    node_t* runner = bt->root;
    node_t* previous = NULL;

    int lequel = 0;

    while(runner != NULL){
        if(strcmp(enWord, runner->enWord) == 0){
            if(runner->left != NULL && runner->right != NULL){
                node_t* previous_new_root = runner;
                node_t* runner_new_root = runner->right;
                int lequel2 = 2;
                while(runner_new_root->left != NULL){
                    previous_new_root = runner_new_root;
                    runner_new_root = runner_new_root->left;
                    lequel2 = 1;
                }
                if(lequel2 == 2){
                    free(runner->enWord);
                    free(runner->frWord);
                    runner->enWord = runner_new_root->enWord;
                    runner->frWord = runner_new_root->frWord;
                    runner->right = runner_new_root->right;
                    free(runner_new_root);
                    return;
                }
                previous_new_root->left = runner_new_root->right;
                free(runner->enWord);
                free(runner->frWord);
                runner->enWord = runner_new_root->enWord;
                runner->frWord = runner_new_root->frWord;
                free(runner_new_root);
                return;
            }
            else if(runner->left != NULL){
                if(previous == NULL){//was root
                    bt->root = runner->left;
                }
                else if(lequel == 1){
                    previous->left = runner->left;
                }
                else{
                    previous->right = runner->left;
                }
            }
            else if(runner->right != NULL){
                if(previous == NULL){//was root
                    bt->root = runner->right;
                }
                else if(lequel == 1){
                    previous->left = runner->right;
                }
                else{
                    previous->right = runner->right;
                }
            }
            else{//no children
                if(previous == NULL){//was root
                    bt->root = NULL;
                }
                else if(lequel == 1){
                    previous->left = NULL;
                }
                else{
                    previous->right = NULL;
                }
            }
            free(runner->enWord);
            free(runner->frWord);
            free(runner);
        return;
        }
        else if(strcmp(enWord, runner->enWord) < 0){
            lequel = 1;
            previous = runner;
            runner = runner->left;
        }
        else{
            lequel = 2;
            previous = runner;
            runner = runner->right;
        }
    }
}
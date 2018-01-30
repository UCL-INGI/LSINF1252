// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void free_bst(node_t* node){
    if(node){
        free_bst(node->right);
        free_bst(node->left);
        free(node);
    }
}

void test_contains_retok1() {
    set_test_metadata("contains", _("Test for the simple returned value"), 1);

    bt_t* tree = (bt_t*) malloc(sizeof(bt_t));
    node_t* root = (node_t*) malloc(sizeof(node_t));
    root->value = 3;
    node_t* node1 = (node_t*) malloc(sizeof(node_t));
    node1->value = 1;
    node_t* node2 = (node_t*) malloc(sizeof(node_t));
    node2->value = 5;
    root->left = node1;
    root->right = node2;
    tree->root = root;
    node_t* node3 = (node_t*) malloc(sizeof(node_t));
    node3->value = 0;
    node_t* node4 = (node_t*) malloc(sizeof(node_t));
    node4->value = 2;
    node1->left = node3;
    node1->right = node4;
    node_t* node5 = (node_t*) malloc(sizeof(node_t));
    node5->value = 4;
    node_t* node6 = (node_t*) malloc(sizeof(node_t));
    node6->value = 6;
    node2->left = node5;
    node2->right = node6;
    int ret = 12320;

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    ret = contains(tree,3);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);

    ret = 0;

    SANDBOX_BEGIN;
    ret = contains(tree,0);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);

    ret = 0;

    SANDBOX_BEGIN;
    ret = contains(tree,6);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);
    CU_ASSERT_EQUAL(stats.malloc.called, 0);
    if (stats.malloc.called) {
      push_info_msg(_("Why did you use malloc ?"));
    }
    CU_ASSERT_EQUAL(stats.free.called, 0);
    if (stats.free.called) {
      push_info_msg(_("Why did you use free ?"));
    }

    free_bst(tree->root);
    free(tree);
}

void test_contains_retok2() {
    set_test_metadata("contains", _("Test for a complex returned value"), 1);

    bt_t* tree = (bt_t*) malloc(sizeof(bt_t));
    node_t* root = (node_t*) malloc(sizeof(node_t));
    root->value = 3;
    node_t* node1 = (node_t*) malloc(sizeof(node_t));
    node1->value = 1;
    node_t* node2 = (node_t*) malloc(sizeof(node_t));
    node2->value = 5;
    root->left = node1;
    root->right = node2;
    tree->root = root;
    node_t* node3 = (node_t*) malloc(sizeof(node_t));
    node3->value = 0;
    node_t* node4 = (node_t*) malloc(sizeof(node_t));
    node4->value = 2;
    node1->left = node3;
    node1->right = node4;
    node_t* node5 = (node_t*) malloc(sizeof(node_t));
    node5->value = 4;
    node_t* node6 = (node_t*) malloc(sizeof(node_t));
    node6->value = 6;
    node2->left = node5;
    node2->right = node6;
    int ret = 1213120;

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    ret = contains(tree,5);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);

    ret = 0;

    SANDBOX_BEGIN;
    ret = contains(tree,2);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);
    CU_ASSERT_EQUAL(stats.malloc.called, 0);
    if (stats.malloc.called) {
      push_info_msg(_("Why did you use malloc ?"));
    }
    CU_ASSERT_EQUAL(stats.free.called, 0);
    if (stats.free.called) {
      push_info_msg(_("Why did you use free ?"));
    }

    free_bst(tree->root);
    free(tree);
}

void test_contains_retnok() {
    set_test_metadata("contains", _("Simple test of the returned value"), 1);

    bt_t* tree = (bt_t*) malloc(sizeof(bt_t));
    node_t* root = (node_t*) malloc(sizeof(node_t));
    root->value = 3;
    node_t* node1 = (node_t*) malloc(sizeof(node_t));
    node1->value = 1;
    node_t* node2 = (node_t*) malloc(sizeof(node_t));
    node2->value = 5;
    root->left = node1;
    root->right = node2;
    tree->root = root;
    node_t* node3 = (node_t*) malloc(sizeof(node_t));
    node3->value = 0;
    node_t* node4 = (node_t*) malloc(sizeof(node_t));
    node4->value = 2;
    node1->left = node3;
    node1->right = node4;
    node_t* node5 = (node_t*) malloc(sizeof(node_t));
    node5->value = 4;
    node_t* node6 = (node_t*) malloc(sizeof(node_t));
    node6->value = 6;
    node2->left = node5;
    node2->right = node6;
    int ret = 122320;

    monitored.malloc = true;
    monitored.free = true;
    printf("%d NB MALLOC\n", stats.malloc.called);
    SANDBOX_BEGIN;
    printf("%d NB MALLOC avant contains\n", stats.malloc.called);
    ret = contains(tree,-2);
    printf("%d NB MALLOC apres contains\n", stats.malloc.called);
    SANDBOX_END;
    printf("%d NB MALLOC\n", stats.malloc.called);

    printf("%d NB MALLOC\n", stats.malloc.called);
    CU_ASSERT_EQUAL(ret,0);

    ret = 0;

    SANDBOX_BEGIN;
    ret = contains(tree,10);
    SANDBOX_END;

    printf("%d NB MALLOC\n", stats.malloc.called);
    CU_ASSERT_EQUAL(ret,0);
    printf("%d NB MALLOC\n", stats.malloc.called);

    CU_ASSERT_EQUAL(stats.malloc.called, 0);
    if (stats.malloc.called) {
      push_info_msg(_("Why did you use malloc ?"));
    }
    CU_ASSERT_EQUAL(stats.free.called, 0);
    if (stats.free.called) {
      push_info_msg(_("Why did you use free ?"));
    }

    free_bst(tree->root);
    free(tree);
}

void test_contains_retnull() {
    set_test_metadata("contains", _("test for empty or null tree"), 1);

    bt_t* tree = (bt_t*) malloc(sizeof(bt_t));
    tree->root = 0;

    int ret = 0;

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    ret = contains(tree,0);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    free(tree);
    tree = 0;

    SANDBOX_BEGIN;
    ret = contains(tree,0);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    CU_ASSERT_EQUAL(stats.malloc.called, 0);
    if (stats.malloc.called) {
      push_info_msg(_("Why did you use malloc ?"));
    }
    CU_ASSERT_EQUAL(stats.free.called, 0);
    if (stats.free.called) {
      push_info_msg(_("Why did you use free ?"));
    }
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    //RUN(test_contains_retok1,test_contains_retok2,test_contains_retnok,test_contains_retnull);
    RUN(test_contains_retnok);
}

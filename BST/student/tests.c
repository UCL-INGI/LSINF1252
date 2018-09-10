#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

bt_t* tree = NULL;

void free_bst(node_t* node){
    if(node){
        free_bst(node->right);
        free_bst(node->left);
        free(node);
    }
}

void test_contains_retok1() {
    set_test_metadata("contains", _("Test contained value"), 1);

    if(!tree) CU_FAIL(_("Internal error while allocating memory"));

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
    };
}

void test_contains_retok2() {
    set_test_metadata("contains", _("Test complex contained value"), 1);

    if(!tree) CU_FAIL(_("Internal error while allocating memory"));

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
}

void test_contains_retnok() {
    set_test_metadata("contains", _("Test not contained value"), 1);

    if(!tree) CU_FAIL(_("Internal error while allocating memory"));

    int ret = 122320;

    monitored.malloc = true;
    monitored.free = true;
    SANDBOX_BEGIN;
    ret = contains(tree,-2);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    ret = 0;

    SANDBOX_BEGIN;
    ret = contains(tree,10);
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

void test_contains_retnull() {
    set_test_metadata("contains", _("Test for empty or null tree"), 1);

    if(!tree) CU_FAIL(_("Internal error while allocating memory"));

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
    tree = (bt_t*) malloc(sizeof(bt_t));
    if(!tree) tree = NULL;
    else {
      node_t* root = (node_t*) malloc(sizeof(node_t));
      node_t* node1 = (node_t*) malloc(sizeof(node_t));
      node_t* node2 = (node_t*) malloc(sizeof(node_t));
      node_t* node3 = (node_t*) malloc(sizeof(node_t));
      node_t* node4 = (node_t*) malloc(sizeof(node_t));
      node_t* node5 = (node_t*) malloc(sizeof(node_t));
      node_t* node6 = (node_t*) malloc(sizeof(node_t));
      if(!root || !node1 || !node2 || !node3 || !node4 || !node5 || !node6){
          if(root) free(root);
          if(node1) free(node1);
          if(node2) free(node2);
          if(node3) free(node3);
          if(node4) free(node4);
          if(node5) free(node5);
          if(node6) free(node6);
          root = NULL; node1 = NULL; node2 = NULL; node3 = NULL; node4 = NULL;
          node5 = NULL; node6 = NULL;
          free(tree);
          tree = NULL;
      }
      else {
          root->value = 3;
          node1->value = 1;
          node2->value = 5;
          root->left = node1;
          root->right = node2;
          tree->root = root;
          node3->value = 0;
          node4->value = 2;
          node1->left = node3;
          node1->right = node4;
          node5->value = 4;
          node6->value = 6;
          node2->left = node5;
          node2->right = node6;
        }
    }

    BAN_FUNCS();
    RUN(test_contains_retok1,test_contains_retok2,test_contains_retnok,test_contains_retnull);

    free_bst(tree->root);
    free(tree);
}

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include "CTester/CTester.h"
#include "student_code.h"

/*
 * Frees all the linked list starting at root
 */
void free_all(student_t* root){
    student_t* runner = root;
    while(runner != NULL){
        root = runner;
        runner = runner->next;
        free(root);
    }
}

/*
 * Creates a linked list of nb elements, of noma's starting at
 * noma_depart, increasing the noma by 1 each time (keeping the same year)
 */
student_t *create_linked_list(int noma_depart, int nb){
    if(nb <= 0)
        return NULL;
    
    student_t* root = (student_t*)malloc(sizeof(student_t));
    if(root == NULL)
        return NULL;
    
    root->noma = noma_depart;
    noma_depart += 10000;
    root->next = NULL;
    
    student_t* previous = root;
    
    int i;
    for(i = 1; i < nb; i++){
        student_t* new = (student_t*)malloc(sizeof(student_t));
        if(new == NULL){
            free_all(root);
            return NULL;
        }
        previous->next = new;
        new->next = NULL;
        new->noma = noma_depart;
        noma_depart += 10000;
        previous = new;
    }
    
    return root;
}

/*
 * Creates/remplaces a file and writes the linked list starting at root 
 */
int create_file(char* filename, student_t* root){
    int fd = open(filename, O_CREAT|O_RDWR, S_IRUSR,S_IWUSR);
    if(fd == -1){
        return -1;
    }
    
    student_t* runner = root;
    while(runner != NULL){
        int error = write(fd, (void*) runner, sizeof(student_t));
        if(error == -1){
            free_all(root);
            close(fd);
            return -1;
        }
        runner = runner->next;
    }
    
    close(fd);
    
    return 0;
}

/*
 * Checks if st1 and st2 contain the same noma's
 */
int same_linked_list(student_t* st1, student_t* st2){
    while(st1 != NULL && st2 != NULL){
        if(st1->noma != st2->noma){
            return 0;
        }
        st1 = st1->next;
        st2 = st2->next;
    }
    if(st1 == NULL && st2 == NULL){
        return 1;
    }
    return 0;
}

/*
 * Test when the call to open fails
 * file created with: student_t* l = create_linked_list(12351600, 5);
 *                   create_file("normal_case.txt", l);
 */
void test_open_fails(){
    set_test_metadata("load_linked_list", _("Test with fail of open"), 1);
    
    student_t* root_ret;
    
    failures.open = FAIL_ALWAYS;
    
    monitored.open = true;
    monitored.close = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("normal_case.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.close = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, NULL);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work when open fails"));
    }
    else{
        set_tag("open");
    }
    
    int memory_used_sol = 0;
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You allocated memory and you did not free it when there is an error"));
    }
    
    if(stats.open.called > 1){
        CU_FAIL();
        push_info_msg(_("You should open only one file"));
    }
    else if(stats.open.called < 1){
        CU_FAIL();
        push_info_msg(_("You have to open the file before using mmap"));
    }
}

/*
 * Test when the file is empty
 * file created with: student_t* l = NULL;
 *                   create_file("one_elemt.txt", l);
 */
void test_empty_file(){
    set_test_metadata("load_linked_list", _("Test with an empty file"), 1);
    
    student_t* root_ret;
    
    monitored.open = true;
    monitored.close = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("empty_file.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.close = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, NULL);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work with an empty file"));
        set_tag("not_expected_list");
    }
    
    int memory_used_sol = 0;
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You allocated memory and you did not free it when there is an error"));
        set_tag("malloc_fail_memory_size");
    }
    
    if(stats.open.called > 1){
        CU_FAIL();
        push_info_msg(_("You should open only one file"));
    }
    else if(stats.open.called < 0){
        CU_FAIL();
        push_info_msg(_("You have to open the file before using mmap"));
    }
    else if(stats.open.called == 1 && stats.close.called == 0){
        CU_FAIL();
        push_info_msg(_("You did not close the file after opening it"));
    }
    else if(stats.open.called == 1 && stats.close.called > 1){
        CU_FAIL();
        push_info_msg(_("Why do you call close more than once ?"));
    }
    else{
        set_tag("close");
    }
}

/*
 * Test when the first call to malloc fails
 * All the memory allocated has to be freed
 * file created with: student_t* l = create_linked_list(46691600, 5);
 *                   create_file("malloc_fails.txt", l);
 */
void test_malloc_fails_first_time(){
    set_test_metadata("load_linked_list", _("Test whith malloc failing first time"), 1);
    
    student_t* root_ret;
    
    failures.malloc = FAIL_FIRST;
    
    monitored.open = true;
    monitored.close = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("malloc_fails.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.close = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, NULL);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Yout function does not work when malloc fails the first time"));
        set_tag("malloc_fail");
    }
    
    int memory_used_sol = 0;
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You allocated memory and you did not free it when there is an error"));
        set_tag("malloc_fail_memory_size");
    }
    
    if(stats.open.called > 1){
        CU_FAIL();
        push_info_msg(_("You should open only one file"));
    }
    else if(stats.open.called < 1){
        CU_FAIL();
        push_info_msg(_("You have to open the file before using mmap"));
    }
    else if(stats.open.called == 1 && stats.close.called == 0){
        CU_FAIL();
        push_info_msg(_("You did not close the file after opening it"));
    }
    else if(stats.open.called == 1 && stats.close.called > 1){
        CU_FAIL();
        push_info_msg(_("Why do you call close more than once ?"));
    }
    else{
        set_tag("close");
    }
}

/*
 * Test when the third call to malloc fails
 * All the memory allocated has to be freed
 * file created with: student_t* l = create_linked_list(46691600, 5);
 *                   create_file("malloc_fails.txt", l);
 */
void test_malloc_fails_third_time(){
    set_test_metadata("load_linked_list", _("Test with malloc failing the third time"), 1);
    
    student_t* root_ret;
    
    failures.malloc = FAIL_THIRD;
    
    monitored.open = true;
    monitored.close = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("malloc_fails.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.close = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, NULL);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work when malloc fails the third time"));
        set_tag("malloc_fail");
    }
    
    int memory_used_sol = 0;
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You allocated memory and you did not free it in case of error"));
        set_tag("malloc_fail_memory_size");
    }
    
    if(stats.open.called > 1){
        CU_FAIL();
        push_info_msg(_("You should open only one file"));
    }
    else if(stats.open.called < 1){
        CU_FAIL();
        push_info_msg(_("You have to open the file before using mmap"));
    }
    else if(stats.open.called == 1 && stats.close.called == 0){
        CU_FAIL();
        push_info_msg(_("You did not close the file after opening it"));
    }
    else if(stats.open.called == 1 && stats.close.called > 1){
        CU_FAIL();
        push_info_msg(_("Why do you call close more than once ?"));
    }
    else{
        set_tag("close");
    }
}

/*
 * Test when the last call to malloc fails
 * last = fifth here because the linked list
 * contains five elements
 * All the memory allocated has to be freed
 * file created with: student_t* l = create_linked_list(46691600, 5);
 *                   create_file("malloc_fails.txt", l);
 */
void test_malloc_fails_last_time(){
    set_test_metadata("load_linked_list", _("Test with malloc failing the last time"), 1);
    
    student_t* root_ret;
    
    failures.malloc = 0b00000000000000000000000000010000;
    
    monitored.open = true;
    monitored.close = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("malloc_fails.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.close = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, NULL);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work when malloc fails for the last malloc"));
        set_tag("malloc_fail");
    }
    
    int memory_used_sol = 0;
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You allocated memory and you did not free it in case of error"));
        set_tag("malloc_fail_memory_size");
    }
    
    if(stats.open.called > 1){
        CU_FAIL();
        push_info_msg(_("You should open only one file"));
    }
    else if(stats.open.called < 1){
        CU_FAIL();
        push_info_msg(_("You should open the file before using mmap"));
    }
    else if(stats.open.called == 1 && stats.close.called == 0){
        CU_FAIL();
        push_info_msg(_("You did not close the file after opening it"));
    }
    else if(stats.open.called == 1 && stats.close.called > 1){
        CU_FAIL();
        push_info_msg(_("Why do you call close more than once ?"));
    }
    else{
        set_tag("close");
    }
}

/*
 * Test when there is only one element in the file
 * one_element.txt: 12691600
 * file created with: student_t* l = create_linked_list(12691600, 1);
 *                   create_file("one_element.txt", l);
 */
void test_one_element(){
    set_test_metadata("load_linked_list", _("Test with a file containing one element"), 1);
    
    int nb_elem = 1;
    
    student_t* sol = create_linked_list(46691600, nb_elem);
    
    student_t* root_ret;
    
    monitored.open = true;
    monitored.close = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("one_element.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.close = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, sol);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work with a file containing one element"));
        set_tag("not_expected_list");
    }
    
    int memory_used_sol = sizeof(student_t) * 1;
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You did not allocate the right amount of memory"));
        set_tag("malloc_fail_memory_size");
    }
    
    if(stats.open.called > 1){
        CU_FAIL();
        push_info_msg(_("You should open onyl one file"));
    }
    else if(stats.open.called < 1){
        CU_FAIL();
        push_info_msg(_("You have to open the file before using mmap"));
    }
    else if(stats.open.called == 1 && stats.close.called == 0){
        CU_FAIL();
        push_info_msg(_("You did not close the file after opening it"));
    }
    else if(stats.open.called == 1 && stats.close.called > 1){
        CU_FAIL();
        push_info_msg(_("Why do you call close more than once ?"));
    }
    else{
        set_tag("close");
    }
    
    free_all(sol);
    free_all(root_ret);
}

/*
 * Test when there are many (5) elements in the file
 * normal_case/txt: 12351600->12361600->12371600->12381600->12391600
 * file created with: student_t* l = create_linked_list(12351600, 5);
 *                   create_file("normal_case.txt", l);
 */
void test_normal_case(){
    set_test_metadata("load_linked_list", _("Test with a file containing many elements"), 1);
    
    int nb_elem = 5;
    
    student_t* sol = create_linked_list(12351600, nb_elem);
    if(sol == NULL){
        return;
    }
    student_t* root_ret;
    
    
    monitored.open = true;
    monitored.close = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("normal_case.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.close = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(sol, root_ret);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your linked list is not what was expected"));
        set_tag("not_expected_list");
    }
    
    int memory_used_sol = sizeof(student_t) * nb_elem;
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You did not allocate the wright amount of memory"));
        set_tag("malloc_fail_memory_size");
    }
    
    if(stats.open.called > 1){
        CU_FAIL();
        push_info_msg(_("You should open only one file"));
    }
    else if(stats.open.called < 1){
        CU_FAIL();
        push_info_msg(_("You have to open the file before using mmap"));
    }
    else if(stats.open.called == 1 && stats.close.called == 0){
        CU_FAIL();
        push_info_msg(_("You did not close the file after opening it"));
    }
    else if(stats.open.called == 1 && stats.close.called > 1){
        CU_FAIL();
        push_info_msg(_("Why do you call close more than once ?"));
    }
    else{
        set_tag("close");
    }
    
    free_all(sol);
    free_all(root_ret);
}

int main(int argc,char* argv[])
{
    BAN_FUNCS(fopen, read, fread, fgetc, fgets);
    RUN(test_open_fails, test_empty_file, test_one_element, test_normal_case, test_malloc_fails_first_time, test_malloc_fails_third_time, test_malloc_fails_last_time);
}

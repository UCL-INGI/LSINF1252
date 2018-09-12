typedef struct student {
    struct student* next;
    char* name;
    int noma;
}

student_t* load_linked_list(char* filename);
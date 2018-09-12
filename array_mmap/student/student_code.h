typedef struct student {
    struct student* next;
    int noma;
} student_t;

student_t* load_linked_list(char* filename);
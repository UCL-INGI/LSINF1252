
typedef struct university {
    char* city;
    int  creation;
    person_t* rector;
} university_t;

typedef struct person {
    char* name;
    int salary;
    int age;
} person_t;


int free_all(university_t* u);
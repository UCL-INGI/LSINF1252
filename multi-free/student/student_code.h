/*
 * If you change the structure, you'll have to change the personal feedback given for the allocation of those structures on the initialisation tests
 */

typedef struct person {
    char* name;
    int salary;
    int age;
} person_t;

typedef struct university {
    char* city;
    int  creation;
    person_t* rector;
} university_t;

university_t* init_all(char* city, int creation, char* rectname, int age, int salary);

void free_all(university_t* u);
// initialise the output test file

void itest_init(const char *name, char *comment);

// close the output test file
void itest_close(char *file);

// report test result
void itest(const char *name, int outcome, char * message);

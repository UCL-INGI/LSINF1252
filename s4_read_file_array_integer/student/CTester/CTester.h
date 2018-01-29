#include <sys/mman.h>
#include <CUnit/CUnit.h>

#include "wrap.h"
#include "trap.h"

#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)

#define RUN(...) void *ptr_tests[] = {__VA_ARGS__}; return run_tests(argc, argv, ptr_tests, sizeof(ptr_tests)/sizeof(void*))
#define BAN_FUNCS(...) 
#define SANDBOX_BEGIN if(sandbox_begin()) { (void)0
#define SANDBOX_END } else { \
                             sandbox_fail(); \
                           } \
                           sandbox_end()


// Hidden by macros
int run_tests(int argc, char *argv[], void *tests[], int nb_tests);
int sandbox_begin();
void sandbox_fail();
void sandbox_end();

// To use inside tests
void set_test_metadata(char *problem, char *descr, unsigned int weight);
void push_info_msg(char *msg);


// Set to true to enable monitoring features
bool wrap_monitoring = false;

struct wrap_stats_t stats;
struct wrap_monitor_t monitored;
struct wrap_fail_t failures;
struct wrap_log_t logs;

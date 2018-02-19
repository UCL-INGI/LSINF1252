#ifndef MATCHERS_H_
#define MATCHERS_H_

#include <clang-c/Index.h>

#define MatcherClangIncludePath "-I/usr/lib/clang/3.4.2/include"

typedef struct matcher_captures {
  size_t size;
  struct matcher_captured_node {
    int is_set;
    CXCursor cursor;
  } *array;
} matcher_captures;

typedef void (*matcher_clean_up_proc)(void *data);
typedef int (*matcher_run_proc)(CXCursor cursor, void *data,
                                matcher_captures *captures);

typedef void (*matcher_visitor)(CXCursor cursor, const char *filename,
                                unsigned int line, unsigned int column,
                                const matcher_captures *captures,
                                void *user_data);

typedef struct matcher {
  matcher_run_proc run;
  matcher_clean_up_proc on_clean_up;

  void *data;
} matcher;

typedef struct matcher_file {
  CXTranslationUnit tu;
  CXIndex index;
} matcher_file;

/*
 * Parses file at path +filename+.
 *
 * The +argv+ array contains arguments that will be passed to clang. It should
 * almost always include +MatcherClangIncludePath+, so that some files like
 * stddef.h can be found.
 */
matcher_file matcher_open(const char *filename, int argc, const char *argv[]);

/*
 * Like +matcher_open+, but prints error messages from Clang if
 * +print_diagnosis+ is non-zero.
 */
matcher_file matcher_open_with_diagnoses(const char *filename,
                                         int argc, const char *argv[],
                                         int print_diagnoses);

/* Closes a file. */
void matcher_close_file(matcher_file f);

/*
 * Searches through +file+ for nodes that are matched by +m+, and returns a
 * non-zero value if it finds at least one.
 *
 * Additionally, the +visitor+ function (if it isn't NULL) will be called for
 * €every node that is matched.
 *
 * This also marks +file+ as the current file.
 */
int matcher_look_for(matcher_file file, matcher m, matcher_visitor visitor,
                     void *user_data);

/*
 * Sets the current file to be used by some functions. This is most often set by
 * using +matcher_look_for+.
 */
void matcher_use_file(matcher_file file);

/*
 * Returns a non-zero value if +matcher+ matches +cursor+.
 * Capture groups will be stored in +captures+ if it isn't NULL.
 */
int matcher_try(matcher matcher, CXCursor cursor, matcher_captures *captures);

/* Releases the memory used by a matcher. */
void matcher_release(matcher m);

typedef enum matcher_binary_operator {
  MatcherOpAdd = (1 << 0), /* + */
  MatcherOpSub = (1 << 1), /* - */
  MatcherOpMul = (1 << 2), /* * */
  MatcherOpDiv = (1 << 3), /* / */
  MatcherOpMod = (1 << 4), /* % */
  MatcherOpLT  = (1 << 5), /* < */
  MatcherOpGT  = (1 << 6), /* > */
  MatcherOpLTE = (1 << 7), /* <= */
  MatcherOpGTE = (1 << 8), /* >= */
  MatcherOpEq  = (1 << 9), /* ** */
  MatcherOpNeq = (1 << 10), /* != */
  MatcherOpLShift = (1 << 11), /* << */
  MatcherOpRShift = (1 << 12), /* >> */
  MatcherOpAssign = (1 << 13), /* = */
} matcher_binary_operator;

typedef enum matcher_unary_operator {
  MatcherOpPreInc     = (1 << 0), /* ++a */
  MatcherOpPostInc    = (1 << 1), /* a++ */
  MatcherOpPreDec     = (1 << 2), /* --a */
  MatcherOpPostDec    = (1 << 3), /* a-- */
  MatcherOpLogicalNeg = (1 << 4), /* !x */
  MatcherOpNegative   = (1 << 5), /* -x */
  MatcherOpPositive   = (1 << 6), /* +x */
  MatcherOpComplement = (1 << 7), /* ~x */
  MatcherOpDeref      = (1 << 8), /* *x */
  MatcherOpAddress    = (1 << 9), /* &x */
} matcher_unary_operator;

/* Always matches. This is most useful when combining matchers. */
matcher match_anything(void);

/* Matches m or simple expressions that use m and evaluate to it (see the
 * README for more details). */
matcher match_result(matcher m);

/* Matches a function called name if one of the nodes that it contains is
 * matched by m. */
matcher match_in_function(const char *name, matcher m);

/* Matches a specific type of node in clang's representation. */
matcher match_node_kind(enum CXCursorKind kind);

/*
 * Matches an array subscript, like array[i]. This does not include the
 * declaration of a new array (e.g. int array[3]).
 */
matcher match_array_subscript(void);

/*
 * Matches a series of operation where there is at least one operand that is a
 * matched by +a+ and another one that is matched by +b+.
 *
 * +op+ is a bitmask formed by OR'ing elements of the
 * +matcher_binary_operator+ enumeration.
 *
 * For examples this expression:
 *
 *     a + b + c + d
 *
 * Could be matched by match_opseq(MatchOpAdd, match_pointer(),
 * match_sizeof()), if at least one of a, b, c, or d is a pointer, and if at
 * least one of the remaining three is a use of sizeof.
 */
matcher match_opseq(int op, matcher a, matcher b);

/*
 * Matches a use of a binary operator where the left hand side matches +lhs+ and
 * the right hand side +rhs+.
 */
matcher match_binop(int op, matcher lhs, matcher rhs);

/* Matches expressions that result in either a pointer or an array. */
matcher match_pointer(void);

/*
 * Matches expressions of type +name+.
 *
 * This uses clang's notion of a canonical type. If you have a structure like:
 *
 *    typedef struct foo { // stuff
 *    } foo;
 *
 * use match_by_typename("struct foo") to refer to a value of type foo and
 * match_by_typename("struct foo *") for a pointer on a value of type foo.
 */
matcher match_by_typename(const char *name);

/* Matches an expression like sizeof(x). */
matcher match_sizeof(void);

/*
 * Matches a call to the function called +name+ with +n+ arguments. The first
 * argument needs to match args[0], the second one args[1], and so on.
 */
matcher match_call(const char *name, size_t n, matcher *args);

/*
 * Matches a reference to a structure member, such as foo->x or foo.x
 *
 * +field_name+ is the name of the field that should be accessed for this to
 * match and +src+ a matcher on the expression that results in the (pointer to
 * the) structure being used.
 */
matcher match_member_ref(matcher src, const char *field_name);

/*
 * Matches when either +a+ or +b+ matches.
 */
matcher match_or(matcher a, matcher b);

/*
 * Matches the declaration of a variable named +name+.
 */
matcher match_var_decl(char *name);

/*
 * Matches whatever +m+ matches and stores the cursor in a capture group with
 * the given id.
 */
matcher match_as(int capture_id, matcher m);

/*
 * Creates a new array to store capture groups.
 */
void matcher_init_captures(matcher_captures *captures);

/*
 * Releases the memory used to store capture groups.
 */
void matcher_release_captures(matcher_captures *captures);

/*
 * Resets all capture groups.
 */
void matcher_reset_captures(matcher_captures *captures);

/*
 * Sets a capture group to be a specific cursor. If there already was one, it is
 * replaced.
 *
 * This returns -1 upon errror, and 0 upon success.
 */
int matcher_set_capture(matcher_captures *captures, int capture_id,
                        CXCursor cursor);

/*
 * Returns the cursor at a capture group id, if there is one, through
 * out_cursor.
 *
 * This function returns 1 if there was a cursor stored at that capture group
 * id, and 0 otherwise.
 */
int matcher_get_capture(const matcher_captures *captures, int capture_id,
                        CXCursor *out_cursor);

/*
 * Returns a non-zero value if the token +token+ is found directly in the
 * cursor (and not one of its children). This can be used to look for reserved
 * key words.
 *
 * This requires the matcher_file that contains +cursor+ to be the current one
 * to work. See +matcher_use_file+.
 */
int matcher_has_token(CXCursor cursor, const char *token);

#endif

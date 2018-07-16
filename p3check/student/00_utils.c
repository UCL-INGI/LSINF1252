#include "tests.h"
#include "matrix.h"

struct sp_matrix *sp_get_empty_matrix(int nlines, int ncols)
{
    struct sp_matrix *m;

    m = malloc(sizeof(*m));
    if (!m)
        return NULL;

    m->lines = NULL;
    m->nlines = nlines;
    m->ncols = ncols;
    m->precision = 0.01;

    return m;
}

struct matrix *get_empty_matrix(int nlines, int ncols)
{
    struct matrix *m;

    m = malloc(sizeof(*m));
    if (!m)
        return NULL;

    m->nlines = nlines;
    m->ncols = ncols;
    m->elems = (double **) malloc(nlines*sizeof(double*));
    
    if(! m->elems) {
        free(m);
    	return NULL;
    }
    
    for(int i=0; i < nlines; i++) {
        m->elems[i] = (double*) calloc(ncols, sizeof(double));
        if(! m->elems[i]) 
        {
            for(int j=0; j<i; j++)
                free(m->elems[j]);
            free(m);
            return NULL;
        } 
    }
    
    return m;
}

static struct elem *get_elem(unsigned int j, int val, struct elem *next)
{
    struct elem *e;

    e = malloc(sizeof(*e));
    if (!e)
        return NULL;

    e->j = j;
    e->value = val;
    e->next = next;

    return e;
}

static struct line *get_line(unsigned int i, struct line *next)
{
    struct line *l;

    l = malloc(sizeof(*l));
    if (!l)
        return NULL;

    l->i = i;
    l->next = next;
    l->elems = NULL;

    return l;
}

struct line *fetch_line(struct sp_matrix *m, unsigned int i)
{
    struct line *l;

    for (l = m->lines; l; l = l->next) {
        if (l->i == i)
            return l;
    }

    return NULL;
}

struct elem *fetch_elem(struct line *l, unsigned int j)
{
    struct elem *e;

    for (e = l->elems; e; e = e->next) {
        if (e->j == j)
            return e;
    }

    return NULL;
}

struct sp_matrix *sp_get_test_matrix1(void)
{
    struct sp_matrix *m;
    struct line *l;
    struct elem *e;

    m = sp_get_empty_matrix(6, 4);
    if (!m)
        return NULL;

    l = get_line(4, NULL);
    e = get_elem(0, 7, get_elem(1, 8, get_elem(3, 9, NULL)));
    l->elems = e;
    l = get_line(2, l);
    e = get_elem(0, 4, get_elem(1, 5, get_elem(2, 6, NULL)));
    l->elems = e;
    l = get_line(1, l);
    e = get_elem(1, 1, get_elem(2, 2, get_elem(3, 3, NULL)));
    l->elems = e;
    m->lines = l;

    return m;
}

struct matrix *get_test_matrix1(void)
{
    struct matrix *m;
    m = get_empty_matrix(6, 4);
    if(!m)
        return NULL;
    
    m->elems[1][1] = 1;
    m->elems[1][2] = 2;
    m->elems[1][3] = 3;
    m->elems[2][0] = 4;
    m->elems[2][1] = 5;
    m->elems[2][2] = 6;
    m->elems[4][0] = 7;
    m->elems[4][1] = 8;
    m->elems[4][3] = 9;
    
    return m;
}

struct sp_matrix *sp_get_test_matrix2(void)
{
    struct sp_matrix *m;
    struct line *l;
    struct elem *e;

    m = sp_get_empty_matrix(6, 4);
    if (!m)
        return NULL;

    l = get_line(4, NULL);
    e = get_elem(0, 7, get_elem(1, 8, get_elem(3, 9, NULL)));
    l->elems = e;
    l = get_line(2, l);
    e = get_elem(0, 4, get_elem(1, -5, get_elem(2, 6, NULL)));
    l->elems = e;
    l = get_line(1, l);
    e = get_elem(1, -1, get_elem(2, -2, get_elem(3, -3, NULL)));
    l->elems = e;
    l = get_line(0, l);
    e = get_elem(1, 9, NULL);
    l->elems = e;
    m->lines = l;

    return m;
}

struct matrix *get_test_matrix2(void)
{
    struct matrix *m;
    m = get_empty_matrix(6, 4);
    if(!m)
        return NULL;
    
    m->elems[0][1] = 9;
    m->elems[1][1] = -1;
    m->elems[1][2] = -2;
    m->elems[1][3] = -3;
    m->elems[2][0] = 4;
    m->elems[2][1] = -5;
    m->elems[2][2] = 6;
    m->elems[4][0] = 7;
    m->elems[4][1] = 8;
    m->elems[4][3] = 9;
    
    return m;
}

struct sp_matrix *sp_get_test_matrix3(void)
{
    struct sp_matrix *m;
    struct line *l;
    struct elem *e;

    m = sp_get_empty_matrix(6, 4);
    if (!m)
        return NULL;

    l = get_line(4, NULL);
    e = get_elem(0, -7, get_elem(1, -8, get_elem(3, -9, NULL)));
    l->elems = e;
    l = get_line(2, l);
    e = get_elem(0, -4, get_elem(1, -5, get_elem(2, -6, NULL)));
    l->elems = e;
    l = get_line(1, l);
    e = get_elem(1, -1, get_elem(2, -2, get_elem(3, -3, NULL)));
    l->elems = e;
    m->lines = l;

    return m;
}

struct matrix *get_test_matrix3(void)
{
    struct matrix *m;
    m = get_empty_matrix(6, 4);
    if(!m)
        return NULL;
    
    m->elems[1][1] = -1;
    m->elems[1][2] = -2;
    m->elems[1][3] = -3;
    m->elems[2][0] = -4;
    m->elems[2][1] = -5;
    m->elems[2][2] = -6;
    m->elems[4][0] = -7;
    m->elems[4][1] = -8;
    m->elems[4][3] = -9;
    
    return m;
}

struct sp_matrix *sp_get_test_matrix4(void)
{
    struct sp_matrix *m;
    struct line *l;
    struct elem *e;

    m = sp_get_empty_matrix(1, 2);
    if (!m)
        return NULL;

    l = get_line(0, NULL);
    e = get_elem(1, 1, NULL);
    l->elems = e;
    m->lines = l;

    return m;
}

struct matrix *get_test_matrix4(void)
{
    struct matrix *m;
    m = get_empty_matrix(1, 2);
    if(!m)
        return NULL;
    
    m->elems[0][1] = 1;
    
    return m;
}

struct sp_matrix *sp_get_test_matrix5(void)
{
    struct sp_matrix *m;
    struct line *l;
    struct elem *e;

    m = sp_get_empty_matrix(4, 6);
    if (!m)
        return NULL;

    l = get_line(3, NULL);
    e = get_elem(1, -3, get_elem(4, 9, NULL));
    l->elems = e;
    l = get_line(2, l);
    e = get_elem(1, -2, get_elem(2, 6, NULL));
    l->elems = e;
    l = get_line(1, l);
    e = get_elem(0, 9, get_elem(1, -1, get_elem(2, -5, get_elem(4, 8, NULL))));
    l->elems = e;
    l = get_line(0, l);
    e = get_elem(2, 4, get_elem(4, 7, NULL));
    l->elems = e;
    m->lines = l;

    return m;
}

struct matrix *get_test_matrix5(void)
{
    struct matrix *m;
    m = get_empty_matrix(4, 6);
    if(!m)
        return NULL;
    
    m->elems[0][2] = 4;
    m->elems[0][4] = 7;
    m->elems[1][0] = 9;
    m->elems[1][1] = -1;
    m->elems[1][2] = -5;
    m->elems[1][4] = 8;
    m->elems[2][1] = -2;
    m->elems[2][2] = 6;
    m->elems[3][1] = -3;
    m->elems[3][4] = 9;
    
    return m;
}

/* Coherency check:
 * - No NULL elems
 * - No zero value
 * - Strict monotonic increase in lines and elems indices
 * - Number of elems/lines within bounds
 * - nlines/ncols > 0
 */

static bool sp_check_no_null_elems(const struct sp_matrix *m)
{
    struct line *l;

    for (l = m->lines; l; l = l->next) {
        if (!l->elems) {
            //pr_err("l->elems is NULL at i = %u", l->i);
            return false;
        }
    }

    return true;
}

static bool sp_check_no_zero_value(const struct sp_matrix *m)
{
    struct line *l;
    struct elem *e;

    for (l = m->lines; l; l = l->next) {
        for (e = l->elems; e; e = e->next) {
            if (fabs(e->value) < fabs(m->precision)) {
                //pr_err("e->value is zero at (i, j) = (%u, %u)", l->i, e->j);
                return false;
            }
        }
    }

    return true;
}

static bool sp_check_monotonic(const struct sp_matrix *m)
{
    int64_t prev_i = -1;
    struct elem *e;
    struct line *l;

    for (l = m->lines; l; l = l->next) {
        int64_t prev_j = -1;

        if (prev_i == -1) {
            prev_i = l->i;
            continue;
        }

        if (l->i <= prev_i) {
            //pr_err("non-monotonic line indices %u -> %u", (unsigned int)prev_i, l->i);
            return false;
        }

        for (e = l->elems; e; e = e->next) {
            if (prev_j == -1) {
                prev_j = e->j;
                continue;
            }

            if (e->j <= prev_j) {
                //pr_err("non-monotonic elem indices %u -> %u at line i = %u",
                //       (unsigned int)prev_j, e->j, l->i);
                return false;
            }
        }
    }

    return true;
}

static bool sp_check_bounds(const struct sp_matrix *m)
{
    struct elem *e;
    struct line *l;
    unsigned int cnt_i = 0;

    for (l = m->lines; l; l = l->next) {
        unsigned int cnt_j = 0;

        cnt_i++;

        for (e = l->elems; e; e = e->next)
            cnt_j++;

        if (cnt_j > m->ncols) {
            //pr_err("number of columns out of bounds (%u > %u) at line i = %u",
            //       (unsigned int)cnt_j, m->ncols, l->i);
            return false;
        }
    }

    if (cnt_i > m->nlines) {
        //pr_err("number of lines out of bounds (%u > %u)", (unsigned int)cnt_i, m->nlines);
        return false;
    }

    return true;
}

static bool sp_check_nonzero_size(const struct sp_matrix *m)
{
    if (!m->nlines) {
        //pr_err("number of lines is zero");
        return false;
    }

    if (!m->ncols) {
        //pr_err("number of columns is zero");
        return false;
    }

    return true;
}

bool sp_check_coherency(struct sp_matrix *m)
{
    if (!sp_check_no_null_elems(m))
        return false;

    if (!sp_check_no_zero_value(m))
        return false;

    if (!sp_check_monotonic(m))
        return false;

    if (!sp_check_bounds(m))
        return false;

    if (!sp_check_nonzero_size(m))
        return false;

    return true;

}
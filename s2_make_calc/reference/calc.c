#include "calc.h"

int main(int argc, char *argv[])
{
    int opt;

    double cur_value;       /* current value */
    int precision;          /* precision for cur_value */

    cur_value = 0.0;        /* default values */
    precision = 0;
    while ((opt = getopt(argc, argv, "a:s:m:d:ID")) != -1) {
        switch (opt) {
            case 'a':
                isValid();
                cur_value += strtod(optarg, NULL);
                break;
            case 's':
                isValid();
                cur_value -= strtod(optarg, NULL);
                break;
            case 'm':
                isValid();
                cur_value *= strtod(optarg, NULL);
                break;
            case 'd':
                isValid();
                cur_value /= strtod(optarg, NULL);
                break;
            case 'I':
                cur_value++;
                break;
            case 'D':
                cur_value--;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-asmd number] [-ID] [precision]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (argv[optind] != NULL) {
        precision = atoi(argv[optind]);
    }
    /* Testing linking with operations.h */
    double a = 1.0;
    double b = 2.0;
    a = calc_add(a, b);

    fprintf(stdout, "%.*f\n", precision, cur_value);
    exit(EXIT_SUCCESS);
}

void isValid() {
    if (strtod(optarg, NULL) == 0) {
        fprintf(stderr, "Error: input is not a valid number\n");
        exit(EXIT_FAILURE);
    }
}

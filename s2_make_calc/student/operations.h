#ifndef __OPERATIONS_H_
#define __OPERATIONS_H_

/**
 * Computes the sum between a and b
 * @pre:    -
 * @post:   -
 * @return: the sum of a and b
 */
double calc_add(double a, double b);

/**
 * Computes the difference between a and b
 * @pre:    -
 * @post:   -
 * @return: the difference of a and b
 */
double calc_sub(double a, double b);

/**
 * Computes the product between a and b
 * @pre:    -
 * @post:   -
 * @return: the product of a and b
 */
double calc_mul(double a, double b);

/**
 * Computes the division between a and b
 * @pre:    b != 0
 * @post:   -
 * @return: the division of a and b
 */
double calc_div(double a, double b);

/**
 * Converts a double to an int
 * @pre:    -
 * @post:   -
 * @return:
 */
int doubleToInt(double number);

// double doublePrecision(double number, int precision);

#endif /* __OPERATIONS_H_ */

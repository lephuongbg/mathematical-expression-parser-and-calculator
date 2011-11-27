/*
 * functions.h
 *
 *  Created on: Nov 22, 2011
 *      Author: herop-kde
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define MAX_DIGITS 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*SOME TYPE OF DATA USING IN THE PROGRAM*/
typedef struct {
	char *number;
	char digits;
	char sign;
} numType;
enum TOKEN {lparen, rparen, oplus, ominus, omultiply, odivide, number, end, omissible};
typedef struct {
	enum TOKEN type;
	numType *value;
} instance;

/*MISCELLANOUS*/
void check_ptr(void *ptr);
//Put each number's digit into one array element
numType *extract(int n);
int num_comp(numType *n1, numType *n2);
int abs_num_comp(numType *n1, numType *n2);
void clone(numType *src, numType *des);
void err();

/*FOR ARITHMETIC*/
numType *multiply(numType *n1, numType *n2);
numType *add(numType *addend1, numType *addend2);
numType *substract(numType *minuend, numType *subtrahend);
numType *divide(numType *dividend, numType *divisor);

/*FOR PARSE MATHEMATICAL EXPRESSIONS*/
instance *lexer(char *string);
numType *parser(char *string);

#endif /* FUNCTIONS_H_ */

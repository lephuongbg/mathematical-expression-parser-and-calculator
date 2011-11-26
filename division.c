/*
 * division.c
 *
 *  Created on: Nov 23, 2011
 *      Author: herop-kde
 */
#include "functions.h"

//Estimate the number of digits for the quotient
int estDigits(numType *dividend, numType *divisor)
{
	int result = 0;
	numType *temp = divisor;
	while (abs_num_comp(dividend,temp) == 1 || abs_num_comp(dividend,temp) == 0)	//compare the dividend with the product of the divisor with 10s
	{
		result++;
		temp = multiply(temp, extract(10));
	}

	return result;
}
typedef struct {
	int *number;
	char digits;
	char sign;
} divType;

/*FOURIER DIVISION ALGORITHM*/
numType *divide(numType *dividend, numType *divisor)
{
	/*INITIALIZE STRUCTURES FOR THE ALGORITHM*/
	//For the new dividend
	numType *new_dividend = NULL;
	new_dividend = realloc(new_dividend, sizeof(numType));
	check_ptr(new_dividend);
	new_dividend->sign = dividend->sign;
	new_dividend->digits = 1 + (dividend->digits+1)/2;
	new_dividend->number = NULL;
	new_dividend->number = realloc(new_dividend->number, new_dividend->digits*sizeof(char));
	check_ptr(new_dividend->number);

	//For the new divisor
	numType *new_divisor = NULL;
	new_divisor = realloc(new_divisor, sizeof(numType));
	check_ptr(new_divisor);
	new_divisor->sign = divisor->sign;
	new_divisor->digits = (estDigits(dividend, divisor)+1)/2;
	new_divisor->number = NULL;
	new_divisor->number = realloc(new_divisor->number, new_divisor->digits*sizeof(char));
	check_ptr(new_divisor->number);

	//For the remainder
	int *remainder = NULL;
	remainder = realloc(remainder, (new_divisor->digits) * sizeof(int));
	check_ptr(remainder);

	//For the temp result type
	divType *temp_result = NULL;
	temp_result = realloc(temp_result, sizeof(divType));
	check_ptr(temp_result);
	temp_result->digits = (estDigits(dividend, divisor)+1)/2;
	temp_result->number = realloc(temp_result->number, temp_result->digits*sizeof(char));
	check_ptr(temp_result->number);

	//For the final result
	numType *final_result = NULL;
	if (estDigits(dividend, divisor) == 0)
	{
		final_result = realloc(final_result, sizeof(numType));
		final_result->number = realloc(final_result->number, sizeof(char));
		final_result->digits = 1;
		final_result->sign = 1;
		return final_result;
	}
	/*INITIALIZATION DONE*/

	/*MIGRATE DATA FROM THE ORIGINAL DIVIDEND AND DIVISOR TO THE NEW ONES*/
	int i, j, k;	//Indices for dividend, divisor and new ones respectively
	for (i = 0, k = 0; i < dividend->digits; i += 2, k++)
	{
		if (i+1 == dividend->digits)
			new_dividend->number[k] = dividend->number[i]*10;
		else
			new_dividend->number[k] = dividend->number[i]*10 + dividend->number[i+1];
	}
	for (i = 0, k = 0; i < divisor->digits; i += 2, k++)
	{
		if (i+1 == divisor->digits)
			new_divisor->number[k] = divisor->number[i]*10;
		else
			new_divisor->number[k] = divisor->number[i]*10 + divisor->number[i+1];
	}

	/*APPLY THE FOURIER ALGORITHM NOW*/
	int temp = 0;
	remainder[0] = (new_dividend->number[0]*100+new_dividend->number[1])%(new_divisor->number[0]);
	temp_result->number[0] = (new_dividend->number[0]*100+new_dividend->number[1])/(new_divisor->number[0]);
	for (k = 1; k < temp_result->digits; k++) //k here is the index for the new result array
	{
		for (j = 1; j <= k; j++)
			temp += temp_result->number[k-j]*new_divisor->number[j];
		remainder[k] = (remainder[k-1]*100 + new_dividend->number[k+1] - temp)%new_divisor->number[0];
		if (remainder[k] < 0)
			remainder[k] += new_divisor->number[0];
		temp_result->number[k] = ((remainder[k-1]*100 + new_dividend->number[k+1] - temp) - remainder[k])/new_divisor->number[0];

		temp = 0;
	}

	//Store the final result
	numType *numTemp = NULL;
	final_result = extract(temp_result->number[0]);
	for (k = 1; k < temp_result->digits; k++)
	{
		numTemp = add(multiply(final_result, extract(100)), extract(temp_result->number[k]));
		free(final_result); final_result = NULL;
		final_result = numTemp;
	}

	final_result->digits = estDigits(dividend, divisor);
	final_result->sign = dividend->sign * divisor->sign;

	free(new_dividend); free(new_divisor); free(temp_result);
	return final_result;
}

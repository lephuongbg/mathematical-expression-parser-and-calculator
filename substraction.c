/*
 * substraction.c
 *
 *  Created on: Nov 23, 2011
 *      Author: herop-kde
 */

#include "functions.h"


void swap(numType **n1, numType **n2)
{
	numType *temp = NULL;
	temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

/*SUBSTRACTION FUNCTION*/
numType *substract(numType *minuend, numType *subtrahend)
{
	/*Initialize result structure*/
	numType *result = NULL;
	result = calloc(1, sizeof(numType));
	check_ptr(result);
	result->number = NULL;
	result->sign = 1;	//Consider default sign

	//Calculate the difference's max number of digits
	int max_len = (minuend->digits > subtrahend->digits) ? minuend->digits : subtrahend->digits;

	//Compare the two numbers
	if (num_comp(minuend, subtrahend) == 0)
	{
		//Immediately return the result when they are equal
		result->number = calloc(1, sizeof(char));
		result->digits = 1;
		result->sign = 1;
		return result;
	} else if (num_comp(minuend, subtrahend) == -1)
	{
		//If the subtrahend is smaller than the minuend,
		result->sign = -1;	//decide the sign
		swap(&minuend, &subtrahend);	//then swap their position
	}

	//Store the max number of digits and initialize digit array
	result->digits = max_len;
	result->number = calloc(max_len, sizeof(char));
	check_ptr(result->number);

	/* Consider different case of the number pair */
	if (minuend->sign == 1 && subtrahend->sign == -1)
	{
		//If the subtrahend is negative
		subtrahend->sign  = 1;	//reverse the subtrahend sign
		return add(minuend, subtrahend);	//then make addition
	} else if (minuend->sign == -1 && subtrahend->sign == -1)
	{
		//If both are negative,
		minuend->sign = 1;	//reverse both's signs
		subtrahend->sign = 1;
		swap(&minuend, &subtrahend);	//then swap their positions
	}

	int i, j, k, carry_up = 0;	//Indices for minuend, subtrahend and difference respectively; and the "carry" variable
	while(i++ < max_len) result->number[i] = 0;
	i = minuend->digits - 1; j = subtrahend->digits - 1; k = max_len - 1;	//Initialize those indices

	for (; k >= 0; i--, j--, k--)
	{
		//result->number[k] += carry_up;	//Do the carry phase first
		minuend->number[i] += carry_up;
		if (j >=0)	//If the subtrahend's still there,
		{
			//Do the substraction from right to left
			if (minuend->number[i] >= subtrahend->number[j])
			{
				result->number[k] += minuend->number[i] - subtrahend->number[j];
				carry_up = 0;
			} else {
				result->number[k] += 10 + minuend->number[i] - subtrahend->number[j];
				carry_up = -1;
				result->number[k] %= 10;
			}
		} else {	//If not,
			if (minuend->number[i] != -1)
			{
				result->number[k] += minuend->number[i]; //bring the minuend's digits down to the result
				carry_up = 0;
			} else
			{
				result->number[k] = 9;
				carry_up = -1;
			}

		}
	}

	//Remove the redundant "0"s at the left of the result
	while (result->number[0] == 0)
	{
		result->number++;
		result->digits--;
	}

	return result;
}

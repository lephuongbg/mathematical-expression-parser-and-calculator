/*
 * addition.c
 *
 *  Created on: Nov 23, 2011
 *      Author: herop-kde
 */
#include "functions.h"

numType *add(numType *addend1, numType *addend2)
{
	/*Initialize result structure*/
	numType *result = NULL;
	int max_len = (addend1->digits>addend2->digits)?(addend1->digits+1):(addend2->digits+1);
	result = malloc(sizeof(numType));
	check_ptr(result);
	result->digits = max_len;		//Store the max number of the sum's digits
	result->sign = 1;	//Default sign
	result->number = NULL;
	result->number = calloc(max_len, sizeof(char));
	check_ptr(result->number);
	//Done initialization

	//Consider different cases of the two number
	if (addend1->sign == -1 && addend2->sign == -1)
		result->sign = -1;	//Only decide the sign and make the normal addition later
	else if (addend1->sign == 1 && addend2->sign == -1)
	{
		addend2->sign = 1;
		return substract(addend1, addend2);
	} else if (addend1->sign == -1 && addend2->sign == 1)
	{
		addend1->sign = 1;
		return substract(addend2, addend1);
	}

	int i, j, k, carry_up = 0;	//Indices for addend 1, addend 2 and the sum with the "carry" variable
	k = -1; while(k++ < max_len) result->number[k] = 0;
	i = addend1->digits - 1; j = addend2->digits - 1; k = max_len - 1;

	for (; k >= 0; i--, j--, k--)	//Sum each pair of digits from right to left
	{
		result->number[k] += carry_up; //Do the carry phase

		if (i < 0 && j >= 0)		//When only addend2's digits are left
			result->number[k] += addend2->number[j];
		else if (j < 0 && i >= 0)	//When only addend1's digits are left
			result->number[k] += addend1->number[i];
		else if (i >= 0 && j >= 0)	//When both digits are left
			result->number[k] += addend1->number[i] + addend2->number[j];
		else		//Addition done
			break;

		carry_up = result->number[k] / 10;	//Make "carry" variable
		result->number[k] %= 10;			//Store only last digit of the calculated sum
	}

	//Remove redundant 0s at the left of the sum
	while (result->number[0] == 0)
	{
		result->number++;
		result->digits--;
	}

	return result;
}

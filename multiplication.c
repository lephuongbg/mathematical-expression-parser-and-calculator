/*
 * multiply.c
 *
 *  Created on: Nov 23, 2011
 *      Author: herop-kde
 */

#include "functions.h"

/*USE LATTICE MULTIPLICATION*/
numType *multiply(numType *n1, numType *n2)
{
	/*Initialize lattice array*/
	struct {
		int a;
		int b;
	} array[MAX_DIGITS][MAX_DIGITS];

	int i, j, temp;
	for (i = 0; i < MAX_DIGITS; i++)
		for (j = 0; j < MAX_DIGITS; j++)
		{
			array[i][j].a = 0;
			array[i][j].b = 0;
		}
	//Initialization done

	//Store all the products of digit pairs (from each factor) into the lattice array
	for (i = 0; i < n2->digits; i++)
		for (j = 0; j < n1->digits; j++)
		{
			temp = n1->number[j] * n2->number[i];
			array[i][j].a = temp / 10;
			array[i][j].b = temp % 10;
		}

	/*Initialize result structure*/
	numType *result = NULL;
	result = realloc(result, sizeof(numType));
	check_ptr(result);
	result->number = NULL;
	result->number = realloc(result->number, (n1->digits + n2->digits) * sizeof(char));
	check_ptr(result->number);

	/*CALCULATE THE PRODUCT*/
	int k;		//The index of the array for the product
	int carry_up = 0;	//For the carry phase
	int during_b = 1;	//Each lattice has two part: a and b. This variable shows the current position being on what part.
	int start_i = n2->digits - 1, start_j = n1->digits - 1;	//The start indices for the two factors' digit array.

	for (k = n1->digits + n2->digits - 1; k >= 0; k--)
	{
		result->number[k] += carry_up;		//Do the carry phase
		if (k <= n2->digits - 1)			//Change the start edge of the lattice array when needed
			during_b = -1;

		i = start_i; j = start_j;
		while (1)
		{
			//Sum all elements on the current diagonal of the lattice
			if (during_b == 1)
				result->number[k] += array[i][j].b;
			else
				result->number[k] += array[i][j].a;

			//Jump to the next lattice
			if (during_b == 1)
				j++;
			else
				i--;
			//Change the lattice's part
			during_b *= -1;

			//When the indices goes out of the array, we break
			if (during_b == -1 && j == n1->digits)
				break;
			else if (during_b == 1 && i == -1)
				break;
		}

		//Make the "carry up" and store only the last digit of the calculated sum
		carry_up = result->number[k] / 10;
		result->number[k] = result->number[k] % 10;

		//Change the start lattice on the edge
		if (k > n2->digits)
			start_j--;
		else if (k < n2->digits)
			start_i--;
		during_b = 1;
	}

	//Store the total number of digits of the result
	result->digits = n1->digits + n2->digits;
	while (result->number[0] == 0)	//Remove redundant "0" at the left of the result
	{
		result->number++;
		result->digits--;
	}

	//Store the sign of the product
	result->sign = n1->sign * n2->sign;

	return result;
}

/*
 * miscellanous.c
 *
 *  Created on: Nov 23, 2011
 *      Author: herop-kde
 */

#include "functions.h"

//Checking valid pointer after dynamic allocating
void check_ptr(void *ptr)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "Failed to initialize an array!");
		free(ptr);
		exit(8);
	}
}

//Return the amount of digits for a number
int digits(int n)
{
	int i = 1;
	while (n >= 10)
	{
		n /= 10;
		i++;
	}
	return i;
}

//Extract number's information into the defined number structure
numType *extract(int input)
{
	numType *n = NULL;
	int i;
	n = malloc(sizeof(numType));
	check_ptr(n);

	//Store the number's sign and number of digits
	if (input < 0)
	{
		n->sign = -1;
		input *= -1;
		n->digits = digits(input);
	} else
	{
		n->sign = 1;
		n->digits = digits(input);
	}

	/*Store each digit into an array's element*/
	//Initialize the array member in the structure
	n->number = NULL;
	n->number = realloc(n->number, digits(input) * sizeof(int));
	if (n->number == NULL)
	{
		fprintf(stderr, "Error creating array!");
		exit(1);
	}
	//Storing digits
	for (i = digits(input) - 1; i >= 0; i--)
	{
		(n->number)[i] = input % 10;
		input /= 10;
	}

	return n;
}

/* COMPARE TWO NUMBER. Return:
 * 1	-	n1 > n2
 * 0	-	n2 = n2
 * -1	-	n1 < n2
 */
int num_comp(numType *n1, numType *n2)
{
	int i, result = 0;

	//Compare the sign of the two number
	if (n1->sign > n2->sign)
		return 1;
	else if (n1->sign < n2->sign)
		return -1;

	//If they have same sign:
	else {
		//Compare the number of digits
		if (n1->digits > n2->digits)
			result = 1;
		else if (n1->digits < n2->digits)
			result = -1;

		//If they have the same number:
		else
		{
			//Compare each digit from left to right
			for (i = 0; i < n1->digits; i++)
				if (n1->number[i] > n2->number[i])
				{
					result = 1;
					break;
				}
				else if (n1->number[i] < n2->number[i])
				{
					result = -1;
					break;
				}
		}
		//Reverse the result when their sign is negative
		if (result != 0)
		{
			if (n1->sign == -1)
				return result*(-1);
			else
				return result;
		}
	}
	return result;
}

//Compare the absolute value of two numbers
int abs_num_comp(numType *n1, numType *n2)
{
	int sign1 = n1->sign;
	int sign2 = n2->sign;
	int result;

	n1->sign = 1; n2->sign = 1;	//Temporary convert the number to positive value
	result = num_comp(n1, n2);
	n1->sign = sign1; n2->sign = sign2; //then now revert their sign

	return result;
}

//Clone a number
void clone(numType *src, numType *des)
{
	(*des).digits = src->digits;
	(*des).sign = src->sign;
	(*des).number = NULL;
	(*des).number = realloc((*des).number, (*des).digits * sizeof(char));
	int i;
	for (i = 0; i < (*des).digits; i++)
		(*des).number[i] = src->number[i];
}

//Error report
void err()
{
	fprintf(stderr, "Invalid mathematical expression!!!\nExiting...\n");
	exit(1);
}

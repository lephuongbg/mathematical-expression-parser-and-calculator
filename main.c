/*
 * BIG CALCULATOR
 *
 *  Created on: Nov 22, 2011
 *      Author: herop-kde
 */

#include "functions.h"

int main()
{
	numType *res2 = NULL;
	int i = 0;
	res2 = parser("300 / 3 + (267 + -7) * 8 + (9 - (10))");

	if (res2->sign == -1)
		printf("-");
	for (i = 0; i < res2->digits; i++)
		printf("%d", res2->number[i]);
	free(res2);
	return 0;
}

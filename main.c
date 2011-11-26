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
	res2 = parser("(2) -(-3) (2 + 8 - 1)");

	if (res2->sign == -1)
		printf("-");
	for (i = 0; i < res2->digits; i++)
		printf("%d", res2->number[i]);
	free(res2);
	return 0;
}

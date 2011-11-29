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
	res2 = parser("11240015742280379851837475397537 * 3749737475758223372036854775808 * 1817272363758463847485 / - 827263484859859 + 8383738495958 * 82732764854959696878873874");
	//res2 = parser("2*(3-7)*(20 - 10) - 56 / 3 + 20 * 3 - 100 * 273");
	if (res2->sign == -1)
		printf("-");
	for (i = 0; i < res2->digits; i++)
		printf("%d", res2->number[i]);
	free(res2);
	return 0;
}

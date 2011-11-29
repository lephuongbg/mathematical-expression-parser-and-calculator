/*
 * BIG CALCULATOR
 *
 *  Created on: Nov 22, 2011
 *      Author: herop-kde
 */

#include "functions.h"

int main()
{
	numType *res = NULL;
	char str[1000];
	int i = 0;
	printf("Type an expression: ");
	fgets(str, sizeof(str), stdin);
	printf("Result: ");

	//Some sample result:
	//res = parser("11240015742280379851837475397537 * 3749737475758223372036854775808 * 1817272363758463847485 / - 827263484859859 + 8383738495958 * 82732764854959696878873874");
	//res = parser("2*(3-7)*(20 - 10) - 56 / 3 + 20 * 3 - 100 * 273");
	res = parser(str);
	if (res->sign == -1)
		printf("-");
	for (i = 0; i < res->digits; i++)
		printf("%d", res->number[i]);
	printf("\n");
	free(res->number);
	free(res);
	return 0;
}

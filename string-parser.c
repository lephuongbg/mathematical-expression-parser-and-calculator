/*
 * string_parser.c
 *
 *  Created on: Nov 25, 2011
 *      Author: herop-kde
 */

#include "functions.h"

/*LEXICAL ANALYSIS FUNCTION
 * Return an array of tokens
 * Each token corresponds to one element the function found from the input string
 * Valid elements: parentheses, operators and numbers*/
instance *lexer(char *string)
{
	instance *lexed=NULL;
	int i = 0; int k = 0;
	int index = 0;
	while (string[i] != '\0')
	{
		switch (string[i])
		{
		case '+':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = oplus;
			lexed[index].value = NULL;
			index++;
			i++;
			break;
		case '-':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = ominus;
			lexed[index].value = NULL;
			index++;
			i++;
			break;
		case '*':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = omultiply;
			lexed[index].value = NULL;
			index++;
			i++;
			break;
		case '/':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = odivide;
			lexed[index].value = NULL;
			index++;
			i++;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = number;
			lexed[index].value = NULL;
			lexed[index].value = realloc((lexed[index]).value, sizeof(numType));
			(lexed[index].value)->digits = 0;
			(lexed[index].value)->sign = 1;
			(lexed[index].value)->number = NULL;
			k = 0;
			while (string[i] >= '0' && string[i] <= '9')
			{
				(lexed[index].value)->digits++;
				(lexed[index].value)->number = realloc((lexed[index].value)->number, (lexed[index].value)->digits*sizeof(char));
				(lexed[index].value)->number[k] = string[i]-'0'; k++;
				i++;
			}
			index++;
			break;
		case '(':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = lparen;
			lexed[index].value = NULL;
			index++;
			i++;
			break;
		case ')':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = rparen;
			lexed[index].value = NULL;
			index++;
			i++;
			break;
		case ' ':
		case '\n':
			i++;
			break;
		default:
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			printf("Unsupported character: %c\n Exiting...\n", string[i]);
			exit(1);
			index++;
			i++;
			break;
		}
	}

	lexed = realloc(lexed, (index + 1) * sizeof(instance));
	check_ptr(lexed);
	lexed[index].type = end;
	lexed[index].value = NULL;
	void post_lexer(instance **);
	post_lexer(&lexed);
	return lexed;
}

/*CHECK THE VALIDITY OF THE LEXICAL ANALYSED RESULT*/
void post_lexer(instance **lexed)
{
	int x = 0;

	/* First check loop */
	int open_paren = 0, close_paren = 0;
	while ((*lexed)[x].type != end)
	{
		//Check for unbalanced parentheses' pairs.
		if ((*lexed)[x].type == lparen)
			open_paren++;
		else if ((*lexed)[x].type == rparen)
			close_paren++;

		//Check for negative or positive number with sign
		if ((*lexed)[x].type == number)
		{
			if (x - 1 >= 0 && ((*lexed)[x-1].type == ominus || (*lexed)[x-1].type == oplus))
			{
				if ((x-2 >= 0 && (*lexed)[x-2].type < number && (*lexed)[x-2].type != rparen) || x-2 < 0)
				{
					if ((*lexed)[x-1].type == ominus)
						(*lexed)[x].value->sign = -1;
					(*lexed)[x-1].type = omissible;
				}
			}
		}
		x++;
	}
	//Report error after first check
	if (open_paren != close_paren)
	{
		fprintf(stderr, "Unbalanced parentheses!!!\nExiting...\n");
		exit(1);
	}

	//Remove omissible element from the array
	int n = x;	//determine the number of elements.
	x = 0; int i = 0; int k;
	while ((*lexed)[x].type != end)
	{
		if ((*lexed)[x].type == omissible)
		{
			for (i = x; i < n; i++)
			{
				(*lexed)[i].type = (*lexed)[i+1].type;
				if ((*lexed)[i].type == number)
				{
					(*lexed)[i].value = NULL;
					(*lexed)[i].value = calloc(1, sizeof(numType));
					(*lexed)[i].value->digits = (*lexed)[i+1].value->digits;
					(*lexed)[i].value->sign = (*lexed)[i+1].value->sign;
					(*lexed)[i].value->number = NULL;
					(*lexed)[i].value->number = calloc((*lexed)[i].value->digits, sizeof(char));
					for (k = 0; k < (*lexed)[i].value->digits; k++)
						(*lexed)[i].value->number[k] = (*lexed)[i+1].value->number[k];
				}
			}
			n--;
			(*lexed) = realloc((*lexed), n * sizeof(instance));
		}
		x++;
	}
	/*STANDARDIZE THE EXPRESSION*/
	// 1) add multiply operators when meeting the implication
	x = 0;
	while ((*lexed)[x].type != end)
	{
		//When encountering multiplying implicated condition
		if (((*lexed)[x].type == number && x+1<n && (*lexed)[x+1].type == lparen) ||			//1) A number beside an open paren
				((*lexed)[x].type == rparen && (*lexed)[x+1].type == lparen) ||					//2) Close paren beside open paren
				((*lexed)[x].type == rparen && (*lexed)[x+1].type == number) ||					//3) Close paren beside a number
				(((*lexed)[x].type == ominus || (*lexed)[x].type == oplus) &&					//4) Minus and plus beside open paren,
				x - 1 > 0 && (*lexed)[x-1].type >= oplus && (*lexed)[x-1].type <= odivide &&	//and after another operator
				x + 1 < n && (*lexed)[x+1].type == lparen))
		{
			//advance every element from that point to the next position
			n++;
			(*lexed) = realloc((*lexed), (n + 1) * sizeof(instance));
			i = n;
			while (i > x + 1)
			{
				(*lexed)[i].type = (*lexed)[i-1].type;
				(*lexed)[i].value = NULL;
				if ((*lexed)[i].type == number)
				{
					(*lexed)[i].value = realloc((*lexed)[i].value, sizeof(numType));
					(*lexed)[i].value->digits = (*lexed)[i-1].value->digits;
					(*lexed)[i].value->sign = (*lexed)[i-1].value->sign;
					(*lexed)[i].value->number = NULL;
					(*lexed)[i].value->number = realloc((*lexed)[i].value->number, (*lexed)[i].value->digits * sizeof(char));
					for (k = 0; k < (*lexed)[i].value->digits; k++)
						(*lexed)[i].value->number[k] = (*lexed)[i-1].value->number[k];
					free((*lexed)[i-1].value); (*lexed)[i-1].value = NULL;
				}
				i--;
			}
			//then add a multiply operator on the suitable position
			(*lexed)[x+1].type = omultiply;
			//If is condition 4: change the operator to a number with a corresponding sign
			if ((*lexed)[x-1].type == ominus || (*lexed)[x-1].type == oplus)
			{
				(*lexed)[x-1].value = NULL;
				if ((*lexed)[x-1].type == ominus)
					(*lexed)[x-1].value = extract(-1);
				else
					(*lexed)[x-1].value = extract(1);
				(*lexed)[x-1].type = number;
			}
		}
		x++;
	}

	//CONSTRUCT THE TIGHT RULES to validate the expression
	x = 0;
	while ((*lexed)[x].type != end)
	{
		switch ((*lexed)[x].type) {
		case lparen:
			if (!((x-1<0)||(x - 1 >= 0 && (*lexed)[x-1].type >= oplus && (*lexed)[x-1].type <= odivide)))
				err();
			if (!(x + 1 < n - 1 && ((*lexed)[x+1].type == number || (*lexed)[x+1].type == lparen)))
				err();
			break;
		case rparen:
			if (!(x - 1 > 0 && ((*lexed)[x-1].type == number || (*lexed)[x-1].type == rparen)))
				err();
			if (!((x+1>=n)||(x + 1 < n  && (*lexed)[x+1].type >= oplus && (*lexed)[x+1].type <= odivide)))
				err();
			break;
		case number:
			if (!((x - 1 < 0)||(x - 1 >= 0 && (*lexed)[x-1].type >= lparen && (*lexed)[x-1].type <= odivide && (*lexed)[x-1].type != rparen)))
				err();
			if (!((x + 1 >= n)||(x + 1 < n && (*lexed)[x+1].type >= rparen && (*lexed)[x+1].type <= odivide)))
				err();
			break;
		case oplus:
		case ominus:
		case omultiply:
		case odivide:
			if (!(x-1 >= 0 && ((*lexed)[x-1].type == number || (*lexed)[x-1].type == rparen)))
				err();
			if (!(x + 1 < n && ((*lexed)[x+1].type == number || (*lexed)[x+1].type == lparen)))
				err();
			break;
		default:
			break;
		}
		x++;
	}

}

/*DO MATHEMATICAL ARITHMETIC BASED ON CORRESPONDING TYPE OF OPERATOR*/
numType *do_math(enum TOKEN op, numType *n1, numType *n2)
{
	numType *result = NULL;
	if (n1->digits < 19 && n2->digits < 19)
	{
		switch (op) {
			case oplus:
				result = extract(compile(n1)+compile(n2));
				break;
			case ominus:
				result = extract(compile(n1)-compile(n2));
				break;
			case omultiply:
				result = extract(compile(n1)*compile(n2));
				break;
			case odivide:
				result = extract(compile(n1)/compile(n2));
				break;
			default:
				break;
		}
	} else {
		switch (op) {
		case oplus:
			result = add(n1, n2);
			break;
		case ominus:
			result = substract(n1, n2);
			break;
		case omultiply:
			result = multiply(n1, n2);
			break;
		case odivide:
			result = divide(n1, n2);
			break;
		default:
			break;
	}
	}
	return	result;		//return NULL upon error
}

/*PARSE THE MATHEMATICAL EXPRESSION AND RETURN THE RESULT*/
//Using the stack-based parsing method
numType *parser(char *string)
{
	instance *lexed = lexer(string); int k = 0;	//First convert the string to an array of tokens
	numType *istack = NULL; int i = 0; 	//Integer stack
	enum TOKEN *cstack = NULL; int c = 0;		//Character stack
	numType *temp = NULL;

	while (lexed[k].type != end)
	{
		if (lexed[k].type == lparen)
		{
			c++;
			cstack = realloc(cstack, c * sizeof(enum TOKEN));
			check_ptr(cstack);
			cstack[c-1] = lexed[k].type;
			k++;
			continue;
		}
		if (lexed[k].type == number)
		{
			i++;
			istack = realloc(istack, i * sizeof(numType));
			check_ptr(istack);
			clone(lexed[k].value, &istack[i-1]);
			k++;
			continue;
		}
		if (lexed[k].type <= odivide && lexed[k].type >= oplus)	//Operator type
		{
			while (c - 1 >= 0 && cstack[c-1]/2 >= lexed[k].type/2 && lexed[k].type/2 != 0)	//Compare priority
			{
				temp = NULL;
				temp = do_math(cstack[c-1], &istack[i-2], &istack[i-1]);
				free(istack[i-2].number);
				clone(temp, &istack[i-2]);
				c--;
				if (c == 0)
				{
					free(cstack); cstack = NULL;
				} else {
					cstack = realloc(cstack, c * sizeof(enum TOKEN));
					check_ptr(cstack);
				}
				free(istack[i-1].number);
				i--;
				istack = realloc(istack, i * sizeof(numType));
				check_ptr(istack);
				if (temp->digits < 19)
					free(temp->number);
				free(temp); temp = NULL;
			}
			c++;
			cstack = realloc(cstack, c * sizeof(enum TOKEN));
			check_ptr(cstack);
			cstack[c-1] = lexed[k].type;
			k++;
			continue;

		}
		if (lexed[k].type == rparen)
		{
			while (cstack[c-1] != lparen)
			{
				temp = NULL;
				temp = do_math(cstack[c-1], &istack[i-2], &istack[i-1]);
				free(istack[i-2].number);
				clone(temp, &istack[i-2]);

				free(istack[i-1].number);
				i--;
				istack = realloc(istack, i * sizeof(numType)); check_ptr(istack);
				if (temp->digits < 19)
					free(temp->number);
				free(temp);
				c--;
				cstack = realloc(cstack, c * sizeof(enum TOKEN)); check_ptr(cstack);
			}
			c--;
			if (c != 0)
			{
				cstack = realloc(cstack, c * sizeof(enum TOKEN));
				check_ptr(cstack);
			} else
			{
				free(cstack); cstack = NULL;
			}
			k++;
			continue;
		}
		if (lexed[k].type == omissible)
		{
			k++;
			continue;
		}
	}

	//Do last operation
	while (c - 1 >= 0)
	{
		temp = NULL;
		temp = do_math(cstack[c - 1], &istack[i - 2], &istack[i - 1]);
		free(istack[i-1].number);
		free(istack[i-2].number);
		clone (temp, &istack[i-2]);
		i--;
		c--;
		if (temp->digits < 19)
			free(temp->number);
		free(temp);
	}

	//For returning result
	temp = calloc(1, sizeof(numType));
	check_ptr(temp);
	clone(&istack[0], temp);
	free(istack[0].number);
	//Release unused memory
	k = 0;
	while (lexed[k].type != end)
	{
		if (lexed[k].type == number)
		{
			free(lexed[k].value->number);
			free(lexed[k].value);
		}
		k++;
	}
	free(istack);
	free(cstack);
	free(lexed);
	return temp;
}

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
			index++;
			i++;
			break;
		case '-':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = ominus;
			index++;
			i++;
			break;
		case '*':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = omultiply;
			index++;
			i++;
			break;
		case '/':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = odivide;
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
			index++;
			i++;
			break;
		case ')':
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			lexed[index].type = rparen;
			index++;
			i++;
			break;
		case ' ':
			i++;
			break;
		default:
			lexed = realloc(lexed, (index + 1) * sizeof(instance));
			check_ptr(lexed);
			//lexed[index].type = unknown;
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
	void post_lexer(instance **);
	post_lexer(&lexed);
	return lexed;
}

/*CHECK THE VALIDITY OF THE LEXICAL ANALYSED RESULT*/
void post_lexer(instance **lexed)
{
	int x = 0;
	//Check for unbalanced parentheses' pairs.
	int open_paren = 0, close_paren = 0;
	while ((*lexed)[x].type != end)
	{
		if ((*lexed)[x].type == lparen)
			open_paren++;
		else if ((*lexed)[x].type == rparen)
			close_paren++;
		x++;
	}
	if (open_paren != close_paren)
	{
		fprintf(stderr, "Unbalanced parentheses!!!\nExiting...\n");
		exit(1);
	}

	x = 0;
	while ((*lexed)[x].type != end)
	{
		if ((*lexed)[x].type == number)
		{
			if (x - 1 >= 0 && ((*lexed)[x-1].type == ominus || (*lexed)[x-1].type == oplus))
			{
				if ((x-2 >= 0 && (*lexed)[x-2].type < number) || x-2 < 0)
				{
					if ((*lexed)[x-1].type == ominus)
						(*lexed)[x].value->sign = -1;
					(*lexed)[x-1].type = omissible;
				}
			}
		}
		x++;
	}

}
/*DO MATHEMATICAL ARITHMETIC BASED ON CORRESPONDING TYPE OF OPERATOR*/
numType *do_math(enum TOKEN op, numType *n1, numType *n2)
{
	if (op == oplus)
		return add(n1, n2);
	if (op == ominus)
		return substract(n1, n2);
	if (op == omultiply)
		return multiply(n1, n2);
	if (op == odivide)
		return divide(n1, n2);

	numType *error = NULL;
	return	error;		//return NULL upon error
}

/*PARSE THE MATHEMATICAL EXPRESSION AND RETURN THE RESULT*/
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
			if (c == 0 || lexed[k].type/2 > cstack[c-1]/2)
			{
				c++;
				cstack = realloc(cstack, c * sizeof(enum TOKEN));
				check_ptr(cstack);
				cstack[c-1] = lexed[k].type;
				k++;
				continue;
			} else {
				temp = do_math(cstack[c-1], &istack[i-2], &istack[i-1]);
				clone(temp, &istack[i-2]);
				i--;
				istack = realloc(istack, i * sizeof(numType));
				check_ptr(istack);
				cstack[c-1] = lexed[k].type;
				k++;
				free(temp);
				continue;
			}
		}
		if (lexed[k].type == rparen)
		{
			while (cstack[c-1] != lparen)
			{
				temp = NULL;
				temp = do_math(cstack[c-1], &istack[i-2], &istack[i-1]);
				clone(temp, &istack[i-2]);
				i--;
				istack = realloc(istack, i * sizeof(numType)); check_ptr(istack);
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
				free(cstack);
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
	while (c != 0)
	{
		temp = do_math(cstack[c-1], &istack[i-2], &istack[i-1]);
		clone (temp, &istack[i-2]);
		i--;
		c--;
	}
	return &istack[0];
}

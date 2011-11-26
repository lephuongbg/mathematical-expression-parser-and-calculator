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

	//Check for negative or positive number with sign
	x = 0;
	while ((*lexed)[x].type != end)
	{
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
					(*lexed)[i].value = realloc((*lexed)[i].value, sizeof(numType));
					(*lexed)[i].value->digits = (*lexed)[i+1].value->digits;
					(*lexed)[i].value->sign = (*lexed)[i+1].value->sign;
					(*lexed)[i].value->number = NULL;
					(*lexed)[i].value->number = realloc((*lexed)[i].value->number, (*lexed)[i].value->digits * sizeof(char));
					for (k = 0; k < (*lexed)[i].value->digits; k++)
						(*lexed)[i].value->number[k] = (*lexed)[i+1].value->number[k];
					(*lexed)[i+1].value = NULL;
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
		if (((*lexed)[x].type == number && x+1<n && (*lexed)[x+1].type == lparen) || ((*lexed)[x].type == rparen && (*lexed)[x+1].type == lparen) || ((*lexed)[x].type == rparen && (*lexed)[x+1].type == number))
		{
			//advance every element from that point to the next position
			n++;
			(*lexed) = realloc((*lexed), n * sizeof(instance));
			i = n - 1;
			while (i > x + 1)
			{
				(*lexed)[i].type = (*lexed)[i-1].type;
				if ((*lexed)[i].type == number)
				{
					(*lexed)[i].value = NULL;
					(*lexed)[i].value = realloc((*lexed)[i].value, sizeof(numType));
					(*lexed)[i].value->digits = (*lexed)[i-1].value->digits;
					(*lexed)[i].value->sign = (*lexed)[i-1].value->sign;
					(*lexed)[i].value->number = NULL;
					(*lexed)[i].value->number = realloc((*lexed)[i].value->number, (*lexed)[i].value->digits * sizeof(char));
					for (k = 0; k < (*lexed)[i].value->digits; k++)
						(*lexed)[i].value->number[k] = (*lexed)[i-1].value->number[k];
					(*lexed)[i-1].value = NULL;
				}
				i--;
			}
			//then add a multiply operator on the current position
			(*lexed)[x+1].type = omultiply;
		}
		x++;
	}
	// 2) convert the + and - before an open parentheses and after another operator to +1 and -1
	x = 0;
	while ((*lexed)[x].type != end)
	{
		if ((*lexed)[x].type == ominus || (*lexed)[x].type == oplus)
			if (x - 1 > 0 && (*lexed)[x-1].type >= oplus && (*lexed)[x-1].type <= odivide)
				if (x + 1 < n && (*lexed)[x+1].type == lparen)
				{
					//advance every element from that point to the next position
					n++;
					(*lexed) = realloc((*lexed), n * sizeof(instance));
					check_ptr(*lexed);
					i = n;
					while (i > x + 1)
					{
						(*lexed)[i].type = (*lexed)[i-1].type;
						if ((*lexed)[i].type == number)
						{
							(*lexed)[i].value = NULL;
							(*lexed)[i].value = realloc((*lexed)[i].value, sizeof(numType));
							(*lexed)[i].value->digits = (*lexed)[i-1].value->digits;
							(*lexed)[i].value->sign = (*lexed)[i-1].value->sign;
							(*lexed)[i].value->number = NULL;
							(*lexed)[i].value->number = realloc((*lexed)[i].value->number, (*lexed)[i].value->digits * sizeof(char));
							for (k = 0; k < (*lexed)[i].value->digits; k++)
								(*lexed)[i].value->number[k] = (*lexed)[i-1].value->number[k];
							(*lexed)[i-1].value = NULL;
						}
						i--;
					}
					//then add a multiply operator on the next position
					(*lexed)[x+1].type = omultiply;
					(*lexed)[x].value = NULL;
					if ((*lexed)[x].type == ominus)
						(*lexed)[x].value = extract(-1);
					else
						(*lexed)[x].value = extract(1);
					(*lexed)[x].type = number;
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
				temp = do_math(cstack[c-1], &istack[i-2], &istack[i-1]);
				clone(temp, &istack[i-2]);
				c--;
				if (c == 0)
				{
					free(cstack); cstack = NULL;
				} else {
					cstack = realloc(cstack, c * sizeof(enum TOKEN));
					check_ptr(cstack);
				}
				i--;
				istack = realloc(istack, i * sizeof(numType));
				check_ptr(istack);
				free(temp);
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
		temp = do_math(cstack[c - 1], &istack[i - 2], &istack[i - 1]);
		clone (temp, &istack[i-2]);
		i--;
		c--;
	}
	return &istack[0];
}

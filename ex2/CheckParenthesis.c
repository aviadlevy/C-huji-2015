/**
* program that check correction fo file's bracket
*
* @Author - Aviad Levy
* @CS Login - aviadle
*/
#include <stdio.h>
#include <unistd.h>

#define BAD_STRUC 1
#define OPEN_FILE_ERROR 2
#define WRONG_PARAM 3

#define USER_FILE argv[1]
#define NUM_OF_PARAM 2

#define MAX_NUMBER_OF_BRACKET 10000

int printError(int typeOfError, char *strToUse);

/**
* check if parenthesis is in the right order using stack method
*/
int main(int argc, char *argv[])
{
	char stackBracket[MAX_NUMBER_OF_BRACKET], c;
	int i = 0;
	FILE *parFile;

	if (argc != NUM_OF_PARAM)
	{
		return printError(WRONG_PARAM, NULL);
	}

	parFile = fopen(USER_FILE, "r");
	if (parFile == NULL)
	{
		return printError(OPEN_FILE_ERROR, USER_FILE);
	}

	while ((c = fgetc(parFile)) != EOF)
	{
		if (c == '(' || c == '[' || c == '<' || c == '{')  //opens new bracket
		{
			stackBracket[i] = c;
			i++;
		}
		else if (c == ')' || c == ']' || c == '>' || c == '}')
		{
			switch (c)   //check if we last open a specific bracket before we can close it
			{
				case ')':
				{
					if (stackBracket[i - 1] == '(')
					{
						stackBracket[i--] = '\0';
					}
					else
					{
						return printError(BAD_STRUC, NULL);
					}
					break;
				}
				case ']':
				{
					if (stackBracket[i - 1] == '[')
					{
						stackBracket[i--] = '\0';
					}
					else
					{
						return printError(BAD_STRUC, NULL);
					}
					break;
				}
				case '>':
				{
					if (stackBracket[i - 1] == '<')
					{
						stackBracket[i--] = '\0';
					}
					else
					{
						return printError(BAD_STRUC, NULL);
					}
					break;
				}
				case '}':
				{
					if (stackBracket[i - 1] == '{')
					{
						stackBracket[i--] = '\0';
					}
					else
					{
						return printError(BAD_STRUC, NULL);
					}
					break;
				}
			}
		}
	}
	fclose(parFile);
	if (i != 0)
	{
		return printError(BAD_STRUC, NULL);
	}
	printf("Ok\n");
	return 0;
}

/**
* responsible for all the error printing in the program
*/
int printError(int typeOfError, char *strToUse)
{
	switch (typeOfError)
	{
		case BAD_STRUC:
		{
			printf("Bad structure\n");
			break;
		}
		case OPEN_FILE_ERROR:
		{
			printf("file \"%s\" not found\n", strToUse);
			break;
		}
		case WRONG_PARAM:
		{
			printf("please supply a file!\nusage:CheckParenthesis <filename>\n");
			break;
		}
		default:
		{
			break;
		}
	}
	return 0;
}


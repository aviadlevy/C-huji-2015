/**
 *
 * shift.c - Program to encrypt or decrypt characters sequence.
 * Author - Aviad Levy
 * CS user - aviadle
 *
 */
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define MAX_SHIFT 25
#define MIN_SHIFT 0

/**
* the func gets char and return TRUE (1) if it's number/letter, and FALSE (0) otherwise.
*/
int isNumberOrLetter(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || \
			(c >= 'a' && c <= 'z') ? TRUE : FALSE;
}

/**
* the func gets the shift and a char to shift, and returns the char after shifting
*/
char encrypt(int shift, char c)
{
	int markBreak = FALSE;
	if (!isNumberOrLetter(c))
	{
		return c;
	}
	int i;
	for (i = shift; i > 0; )
	{
		switch (c)
		{
			case 'z':

				i--;
				c = 'a';
				if (i == 0)
				{
					markBreak = TRUE;
				}
				break;

			case 'Z':

				i--;
				c = 'A';
				if (i == 0)
				{
					markBreak = TRUE;
				}
				break;

			case '9':
				i--;
				c = '0';
				if (i == 0)
				{
					markBreak = TRUE;
				}
				break;

			default:
			{
				c++;
				i--;
				break;
			}
		}
		if (markBreak)
		{
			break;
		}
	}
	return c;
}

/**
* the func gets the shift and a char after shifting, and returns the original char before shifting
*/
char decrypt(int shift, char c)
{
	int markBreak = FALSE;
	if (!isNumberOrLetter(c))
	{
		return c;
	}
	int i;
	for (i = shift; i > 0; )
	{
		switch (c)
		{
			case 'a':
				i--;
				c = 'z';
				if (i == 0)
				{
					markBreak = TRUE;
				}
				break;

			case 'A':
				i--;
				c = 'Z';
				if (i == 0)
				{
					markBreak = TRUE;
				}
				break;

			case '0':
				i--;
				c = '9';
				if (i == 0)
				{
					markBreak = TRUE;
				}
				break;

			default:
			{
				c--;
				i--;
				break;
			}
		}
		if (markBreak)
		{
			break;
		}
	}
	return c;
}

/**
* main function to deal with input and output.
* the function gets a chars and the shift to encrypt/decrypt, and prints the chars after
* encrypt/decrypt.
*/
int main()
{
	char c;
	int shift = 0, choose = 0, userTyping = 0;
	printf("please enter the shift:\n");
	scanf("%d", &shift);
	//check if shift is a legal input
	if(shift > MAX_SHIFT || shift < MIN_SHIFT)
	{
		printf("Worng input. Shift has to be between 0 to 25\nExiting...");
		return 0;
	}
	//loop to verify that the user entered a legal input ('e'/'d')
	while (choose != 'd' && choose != 'e')
	{
		printf("would you like to encrypt (e) or decrypt (d)?\n");
		scanf("%s", &choose);   //check all the string entered, and allow only 1 legal char ("dd"
								// will not work)
	}
	scanf("%c", &userTyping);
	if (choose == 'e')
	{
		while((userTyping = getchar()) != EOF)
		{
			printf("%c", encrypt(shift, (char) userTyping));
		}
	}
	if (choose == 'd')
	{
		while((userTyping = getchar()) != EOF)
		{
			printf("%c", decrypt(shift, (char) userTyping));
		}
	}
	printf("\n");
	return 0;
}

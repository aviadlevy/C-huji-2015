/**
 *
 * count.c - Program to count number of characters, words and lines the user entered.s
 * Author - Aviad Levy
 * CS user - aviadle
 *
 */
#include <stdio.h>

/**
* the function iterate over the user input and count chars, words and lines.
* then it prints the results.
*/
int main()
{
	int charCounter = 0, wordCounter = 0, lineCounter = 1;
	int c, prevC = 0;    // the integers that will use as chars, one for current char and one for previous
	while ((c = getchar()) != EOF)
	{
		charCounter++;
		if (c == ' ')
		{
			//add word only if needed (prev char is no space/new line)
			if (prevC != 0 && prevC != ' ' && prevC != '\n')
			{
				wordCounter++;
			}
		}
		else if (c == '\n')
		{
			lineCounter++;
			//add word only if needed (prev char is no space/new line)
			if (prevC != 0 && prevC != ' ' && prevC != '\n')
			{
				wordCounter++;
			}
		}
		prevC = c;
	}
	//add word only if needed (prev char is no space/new line
	if (prevC != 0 && prevC != ' ' && prevC != '\n')
	{
		wordCounter++;
	}
	printf("%d %d %d", charCounter, wordCounter, lineCounter);
	return 0;
}

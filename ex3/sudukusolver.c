/**
* file that contain the main function for solving suduku using dfs
*
* @Author - Aviad Levy
* @CS Login - aviadle
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "sudukutree.h"

#define NUM_OF_PARAM_IN_USAGE 2
#define FILE_ARG argv[1]
#define LINE_LENGTH 200

#define BEST 1

#define UNVALID_FILE 0
#define NO_SOLUTION 1
#define USAGE_ERROR 2
#define FILE_NOT_FOUND 3

#define ERROR_IN_PROCESS -1

int countSpaces(char const *line);

int main(int argc, char *argv[])
{
	FILE *fileToRead;
	sudokuStack tree;

	if (argc != NUM_OF_PARAM_IN_USAGE)
	{
		return printError(USAGE_ERROR, NULL);
	}

	fileToRead = fopen(FILE_ARG, "r");
	if (fileToRead == NULL)
	{
		return printError(FILE_NOT_FOUND, NULL);
	}

	if (processFile(fileToRead, &tree, FILE_ARG) == ERROR_IN_PROCESS)
	{
		return 1;
	}
	fclose(fileToRead);
	pNode best = getBest(&tree, getChildren, getVal, pop, copy, BEST);
	printSudoku(best);
	freeSudokuNode(best);
	return 0;
}

int processFile(FILE *const fileToRead, sudokuStack *stack, char *const fileName)
{
	char line[LINE_LENGTH];
	int i = 0, numOfRows = 0, **tempSudokuArr = NULL;
	for (; fgets(line, LINE_LENGTH, fileToRead) != NULL; i++)
	{
		if (i == 0)
		{
			char *rowChar = strtok(line, "\n");
			numOfRows = atoi(rowChar);
			int j;
			if (checkNumberOfRows(rowChar, fileName))
			{
				return ERROR_IN_PROCESS;
			}
			*stack = sudokuStackAlloc();
			tempSudokuArr = malloc(sizeof(int *) * numOfRows);
			for (j = 0; j < numOfRows; j++)
			{
				(tempSudokuArr)[j] = (int *) malloc(sizeof(int) * numOfRows);
			}
		}
		else
		{
			if (countSpaces(line) != numOfRows - 1)
			{
				printError(UNVALID_FILE, fileName);
				return ERROR_IN_PROCESS;
			}
			char *row;
			row = strtok(line, "\n");
			int j;
			for (j = 0; j < numOfRows; j++)
			{
				char *num;
				if (j == 0)
				{
					num = strtok(row, " ");
				}
				else
				{
					num = strtok(NULL, " ");
				}
				(tempSudokuArr)[i - 1][j] = atoi(num);
				if (checkNumberInSuduku(num, numOfRows, fileName))
				{
					return ERROR_IN_PROCESS;
				}
			}
		}
	}
	push(stack, tempSudokuArr, numOfRows);
	for (i = 0; i < numOfRows; i++)
	{
		free(tempSudokuArr[i]);
	}
	free(tempSudokuArr);
	return numOfRows;
}

int countSpaces(char const *line)
{
	int i = 0;
	char c;
	while ((c = *line++) != 0)
	{
		if (c == ' ')
		{
			i++;
		}
	}

	return i;
}

int checkNumberOfRows(char *const rowChar, char *const fileName)
{
	if (checkNumAndNotSomthingElse(rowChar))
	{
		return printError(UNVALID_FILE, fileName);
	}
	int numOfRows = atoi(rowChar);
	if (!checkIfSqrtIsInt(numOfRows))
	{
		return (printError(UNVALID_FILE, fileName));
	}
	return 0;

}

int checkNumberInSuduku(char *const numChar, int const numOfRows, char *const fileName)
{
	if (checkNumAndNotSomthingElse(numChar))
	{
		return printError(UNVALID_FILE, fileName);
	}
	int num = atoi(numChar);
	if (num > numOfRows)
	{
		return printError(UNVALID_FILE, fileName);
	}
	return 0;
}

void push(sudokuStack *stack, int **const sudokuToPush, int const numOfRow)
{
	sudokuNode *node = (sudokuNode *) malloc(sizeof(sudokuNode));
	node->sudukuArr = malloc(sizeof(int *) * numOfRow);
	int i;
	for (i = 0; i < numOfRow; i++)
	{
		node->sudukuArr[i] = malloc(sizeof(int) * numOfRow);
	}
	copySudokuArr(node->sudukuArr, sudokuToPush, numOfRow);
	node->numOfRow = numOfRow;
	node->next = stack->head;
	stack->head = node;
}


void copySudokuArr(int **sudokuArr, int **const push, int const numOfRow)
{
	int i, j;
	for (i = 0; i < numOfRow; i++)
	{
		for (j = 0; j < numOfRow; j++)
		{
			sudokuArr[i][j] = push[i][j];
		}
	}
}

sudokuStack sudokuStackAlloc()
{
	sudokuStack *stack = (sudokuStack *) malloc(sizeof(sudokuStack));
	stack->head = NULL;
	return *stack;
}

int checkNumAndNotSomthingElse(char const *num)
{
	unsigned int j;
	for (j = 0; j < strlen(num); j++)
	{
		if (!isdigit(num[j]))
		{
			return 1;
		}
	}
	return 0;
}

int checkIfSqrtIsInt(int const num)
{
	return sqrt(num) == (int) sqrt(num);
}

void printSudoku(pNode node)
{
	if (node == NULL)
	{
		printError(NO_SOLUTION, NULL);
		return;
	}
	sudokuNode *best = node;
	printf("%d\n", best->numOfRow);
	int i, j;
	for (i = 0; i < best->numOfRow; i++)
	{
		for (j = 0; j < best->numOfRow; j++)
		{
			if (j == best->numOfRow - 1)
			{
				printf("%d", best->sudukuArr[i][j]);
				continue;
			}
			printf("%d ", best->sudukuArr[i][j]);
		}
		printf("\n");
	}
}

int printError(int const typeOfError, char *const fileName)
{
	switch (typeOfError)
	{
		case UNVALID_FILE:
		{
			printf("%s: not a valid suduku file\n", fileName);
			break;
		}
		case NO_SOLUTION:
		{
			printf("no solution!\n");
			break;
		}
		case USAGE_ERROR:
		{
			printf("please supply a file!\nusage: sudukusolver <filename>");
			break;
		}
		case FILE_NOT_FOUND:
		{
			printf("%s: no such file\n", fileName);
			break;
		}
		default:
		{
			break;
		}
	}
	return 1;
}

/**
* file that contain the function that delivered to the function getBest (in genericdfs)
*
* @Author - Aviad Levy
* @CS Login - aviadle
*/

#include <stdlib.h>
#include <math.h>
#include "sudukutree.h"

void pop(pNode node)
{
	sudokuStack *stack = node;
	if (stack == NULL)
	{
		return;
	}
	if (stack->head == NULL)
	{
		return;
	}
	sudokuNode *nodeToFree = stack->head;
	stack->head = nodeToFree->next;
	freeSudokuNode(nodeToFree);
}

void freeSudokuNode(sudokuNode *node)
{
	int i;
	if (node != NULL)
	{
		for (i = 0; i < node->numOfRow; i++)  //free 2D array
		{
			free(node->sudukuArr[i]);
		}
		free(node->sudukuArr);
	}
	free(node);
}

void *copy(pNode pNodeToCopy)
{
	sudokuStack *nodeToCopy = pNodeToCopy;
	sudokuNode *node = malloc(sizeof(*nodeToCopy->head));
	node->isFinish = nodeToCopy->head->isFinish;
	node->numOfRow = nodeToCopy->head->numOfRow;
	node->sudukuArr = malloc(sizeof(int *) * nodeToCopy->head->numOfRow);
	int i;
	for (i = 0; i < node->numOfRow; i++)
	{
		node->sudukuArr[i] = malloc(sizeof(int) * node->numOfRow);
	}
	copySudokuArr(node->sudukuArr, nodeToCopy->head->sudukuArr, node->numOfRow);
	return node;
}

int getChildren(pNode pNodeTree, pNode **treeToReturn)
{
	sudokuStack *tree = pNodeTree;
	int *location;
	int numOfChildren = 0;
	location = findUnassignedLocation(tree->head->sudukuArr, tree->head->numOfRow);
	if (location[0] == tree->head->numOfRow + 1 && location[1] == tree->head->numOfRow + 1)
	{
		if(numOfChildren == 0)  // the user entered a complete pazel
		{
			*treeToReturn = (pNode *) tree;
		}
		tree->head->isFinish = 1;
		free(location);
		return 1;
	}
	tree->head->isFinish = 0;
	int row = location[0];
	int col = location[1];
	free(location);
	int j;
	for (j = 1; j <= tree->head->numOfRow; j++)
	{
		if (isAvailable(tree->head->sudukuArr, row, col, j, tree->head->numOfRow))
		{
			push(tree, tree->head->sudukuArr, tree->head->numOfRow);
			tree->head->sudukuArr[row][col] = j;
			if (getChildren(tree, treeToReturn) != -1)
			{
				if (tree->head->isFinish == 1)
				{
					*treeToReturn = (pNode *) tree;
				}
				numOfChildren++;
				return numOfChildren;
			}
			tree->head->sudukuArr[row][col] = UNASSIGNED;
		}
	}
	return ERROR_DURING_GET_CHILDREN;
}

unsigned int getVal(pNode node)
{
	sudokuStack *convertedNode = node;
	return (unsigned int) convertedNode->head->isFinish;
}

int *findUnassignedLocation(int **const sudukuArr, int const numOfRow)
{
	int row, col;
	int* location = malloc(sizeof(int) * 2);
	location[0] = numOfRow + 1;
	location[1] = numOfRow + 1;
	for (row = 0; row < numOfRow; row++)
	{
		for (col = 0; col < numOfRow; col++)
		{
			if (sudukuArr[row][col] == UNASSIGNED)
			{

				location[0] = row;
				location[1] = col;
				return location;
			}
		}
	}
	return location;
}

int isAvailable(int **const grid, int const row, int const col, int const num, int const numOfRow)
{
	return !usedInRow(grid, row, num, numOfRow) && !usedInCol(grid, col, num, numOfRow)
			&& !usedInBox(grid, row, col, num, numOfRow);
}

int usedInBox(int **const grid, int const row, int const col, int const num, int const numOfRow)
{
	int boxStartRow = row - row % (int) sqrt(numOfRow);
	int boxStartCol = col - col % (int) sqrt(numOfRow);
	int rowIter, colIter;
	for (rowIter = 0; rowIter < (int) sqrt(numOfRow); rowIter++)
	{
		for (colIter = 0; colIter < (int) sqrt(numOfRow); colIter++)
		{
			if (grid[rowIter + boxStartRow][colIter + boxStartCol] == num)
			{
				return 1;
			}
		}
	}
	return 0;
}

int usedInCol(int **const grid, int const col, int const num, int const numOfRow)
{
	int row;
	for (row = 0; row < numOfRow; row++)
	{
		if (grid[row][col] == num)
		{
			return 1;
		}
	}
	return 0;
}

int usedInRow(int **const grid, int const row, int const num, int const numOfRow)
{
	int col;
	for (col = 0; col < numOfRow; col++)
	{
		if (grid[row][col] == num)
		{
			return 1;
		}
	}
	return 0;
}

/**
* file that contain the header for all deceleration used in suduku files
*
* @Author - Aviad Levy
* @CS Login - aviadle
*/

#include <stdio.h>
#include <wchar.h>
#include "genericdfs.h"

#define UNASSIGNED 0
#define ERROR_DURING_GET_CHILDREN -1  //must be negative!

/**
* node that hold:
*
* @param sudukuArr 2d array of suduku pazel
* @param next pointer to next node
* @param numOfRow number of rows in suduku
* @param isFinish 1 if the suduku is finished, 0 otherwise
*/
typedef struct sudokuNode
{
	int **sudukuArr;
	struct sudokuNode *next;
	int numOfRow;
	int isFinish;
} sudokuNode;

/**
* stack that hold  the sudukuNode
*
* @param head the head of the stack
*/
typedef struct sudokuStack
{
	sudokuNode *head;
} sudokuStack;

/**
* function that get the val of each node.
* in our case, check if the suduku is finish
*
* @node pointer to suduku node
* @return the value of the node
*/
unsigned int getVal(pNode node);

/**
* A function that gets a node and a pointer to an array of nodes.
* The function allocates memory for an array of all the children of the node, populate it (solve
* the suduku), and returns it using the second parameter. The returned value is the number of
* children.
*
* @param tree the first node that we need to build the suduku from
* @param treeToReturn the tree after we built it
* @return the number of children of the tree
*/
int getChildren(pNode tree, pNode **treeToReturn);

/**
* process the file the user gave us, and enter the processed objects to head of stack, after we
* allocate the memory.
*
* @param fileToRead the file the user gave us
* @param stack the stack we building
*/
int processFile(FILE *const fileToRead, sudokuStack *stack, char *const fileName);

/**
* function that get 2d suduku array and check the next unassigned location
*
* @param sudukuArr the suduku pazel we check
* @param numOfRow the number of rows in suduku
* @return array of 2 ints when the 1st cell is the row number and the 2nd is th col
*/
int *findUnassignedLocation(int **const sudukuArr, int const numOfRow);

/**
* function that check if number is available according to suduku rules
*
* @param grid 2d array of suduku
* @param row the row we check
* @param col the column we check
* @param num the number we want to enter
* @param numOfRow the number of rows in suduku
* @return 1 if the number is legal, 0 otherwise
*/
int isAvailable(int **const grid, int const row, int const col, int const num,
		int const numOfRow);

/**
* function that check if number is available in row according to suduku rules
*
* @param grid 2d array of suduku
* @param row the row we check
* @param num the number we want to enter
* @param numOfRow the number of rows in suduku
* @return 1 if the number is legal, 0 otherwise
*/
int usedInRow(int **const grid, int const row, int const num, int const numOfRow);

/**
* function that check if number is available in column according to suduku rules
*
* @param grid 2d array of suduku
* @param col the column we check
* @param num the number we want to enter
* @param numOfRow the number of rows in suduku
* @return 1 if the number is legal, 0 otherwise
*/
int usedInCol(int **const grid, int const col, int const num, int const numOfRow);

/**
* function that check if number is available in box according to suduku rules
*
* @param grid 2d array of suduku
* @param row the row we check
* @param col the column we check
* @param num the number we want to enter
* @param numOfRow the number of rows in suduku
* @return 1 if the number is legal, 0 otherwise
*/
int usedInBox(int **const grid, int const row, int const col, int const num, int const numOfRow);

/**
* function that creat and create place in memory for new sudukuStack
*
* @return the stack we created
*/
sudokuStack sudokuStackAlloc();

/**
* function that copy 2d array to other 2d arrat
*
* @param sudukuArr empty 2d array of ints we want to copy to
* @param push 2d array we want to copy to sudukuArr
* @param numOfRow the number of rows in the suduku
*/
void copySudokuArr(int **sudokuArr, int **const push, int const numOfRow);

/**
* function that push sudukuNode to stack given
*
* @param stack the stack we push to the suduku
* @param sudukuToPush the 2d array we want to push
* @param numOfRow the number of rows in the suduku
*/
void push(sudokuStack *stack, int **const sudokuToPush, int const numOfRow);

/**
* function the free a node from a stack. we'll send if to getBest as the freeNode requested
*
* @param node the node we want to pop
*/
void pop(pNode stack);

/**
* function that free the node given
*
* @param node the node to free
*/
void freeSudokuNode(sudokuNode *node);

/**
* function that deep copy suduku node
*
* @param nodeToCopy node to deep copy
* @return pointer to the new node
*/
void *copy(pNode nodeToCopy);

/**
* function that print suduku pazzel
*
* @param node pointer to node that hold suduku 2d array
*/
void printSudoku(pNode node);

/**
* the function get the type of error and print the massage that match the error
*
* @param typeError the type of error that happend
*/
int printError(int const typeOfError, char *const fileName);

/**
* function that check if the number is remain natural (tivee) after sqrt (shoresh)
*
* @param num the number to check
* @return 1 if true, 0 otherwise
*/
int checkIfSqrtIsInt(int const num);

/**
* the function get an array of chars and check each char if is number or not
*
* @param num the array we check
* @return 1 if a char of non digit is found, 0 if all chars is digit
*/
int checkNumAndNotSomthingElse(char const *num);

/**
* function that check if the number of rows (in the head of the file) is legal
*
* @param rowChar the number to check
* @return 1 if no legal, 0 otherwise
*/
int checkNumberOfRows(char *const rowChar, char *const fileName);

/**
* function that check if every number of the suduku is legal
*
* @param numChar the number to check
* @param numOfRows number of rows in suduku
* @return 1 if no legal, 0 otherwise
*/
int checkNumberInSuduku(char *const numChar, int const numOfRows, char *const fileName);


/**
* program that manage warehouse orders, sending and eve cleaning
*
* @Author - Aviad Levy
* @CS Login - aviadle
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DB_FILE_ARG argv[1]
#define USER_COMMAND argv[2]
#define FILE_TO_READ_ARG argv[3]

#define YEAR date[0]
#define MONTH date[1]

#define NUM_OF_PARAM_IN_USAGE 4

#define ERROR_RETURN -1

#define WRONG_PARAM 1
#define OPEN_FILE_ERROR 2
#define UNKNOWN_FORMAT 3
#define PERMISSION_ERROR 4
#define NOT_ENOUGH 5

#define RECEIVED "received"
#define SENT "sent"
#define CLEAN "clean"

#define MAX_LINE_NUMBER 2000
#define LINE_LENGTH 150

#define TYPE_REGULAR 1
#define TYPE_SENT 2
#define TYPE_CLEAN 3

#define EMPTY_CHAR '\0'

#define DEVIATION 0.001

//bubble sort we'll use several times later
#define SWAP Product temp = productToPrint[j]; \
			productToPrint[j] = productToPrint[j + 1];  \
			productToPrint[j + 1] = temp;

/**
* struct for the product we process
*/
typedef struct Product
{
	char name[21];
	int barcode;
	float weight;
	int date[2];  //year and month. see macro
} Product;

/**
* special struct for checking input
*/
typedef struct StringProduct
{
	char *name;
	char *barcode;
	char *weight;
	char *year;
	char *month;
} StringProduct;

int printError(int errorType);

int fileToProduct(Product *product, FILE *fileToRead);

int checkingInput(StringProduct product, int type);

int fileToProductSend(Product *product, FILE *fileToRead);

void sortAndInsertToFile(Product *productDB, int numOfProductInDB, Product *productFILE,
						 int numOfProductReceived, FILE *fileToWrite);

int sent(Product *productToRead, Product *productInDB,
		 int numOfProductInDB, int numOfProductToSend);

int received(Product *productToRead, Product *productInDB,
			 int numOfProductInDB, int numOfProductReceived);

int clean(char *userInput, Product *productToRead, Product *productInDB, int numOfProductInDB);

int checkFile(FILE *fileToCheck);

/**
* main function to get the inputs file, and edit them in order to user request
*/
int main(int argc, char *argv[])
{
	FILE *fileOfDB, *fileToRead, *fileToWrite;
	Product productToRead[1000], productInDB[1000];

	if (argc != NUM_OF_PARAM_IN_USAGE)
	{
		return printError(WRONG_PARAM);
	}
	//check if clean. if so skip opening file and except string
	if (strcmp(USER_COMMAND, CLEAN))
	{
		fileToRead = fopen(FILE_TO_READ_ARG, "r");
		if (checkFile(fileToRead))
		{
			return 1;
		}
	}

	fileOfDB = fopen(DB_FILE_ARG, "r");
	if (checkFile(fileOfDB))
	{
		return 1;
	}
	//process DB file
	int numOfProductInDB = fileToProduct(productInDB, fileOfDB);
	if (numOfProductInDB == ERROR_RETURN)
	{
		return 1;
	}
	fclose(fileOfDB);

	if (!strcmp(USER_COMMAND, RECEIVED))
	{

		int numOfProductReceived = fileToProduct(productToRead, fileToRead);
		int isEverythingOK = received(productToRead, productInDB, numOfProductInDB,	numOfProductReceived);
		if (isEverythingOK)
		{
			return 1;
		}
		//if everything went OK, let's write to file
		fileToWrite = fopen(DB_FILE_ARG, "w");
		if (checkFile(fileToWrite))
		{
			return 1;
		}
		sortAndInsertToFile(productInDB, numOfProductInDB, productToRead, numOfProductReceived,
				fileToWrite);
	}

	else if (!strcmp(USER_COMMAND, SENT))
	{
		int numOfProductToSend = fileToProductSend(productToRead, fileToRead);
		int everythingOK = sent(productToRead, productInDB, numOfProductInDB, numOfProductToSend);
		if (everythingOK)
		{
			return 1;
		}

		//if everything went OK, let's write to file
		fileToWrite = fopen(DB_FILE_ARG, "w");
		if (checkFile(fileToWrite))
		{
			return 1;
		}
		sortAndInsertToFile(productInDB, numOfProductInDB, NULL, 0, fileToWrite);
	}

	else if (!strcmp(USER_COMMAND, CLEAN))
	{
		int everythingOK = clean(FILE_TO_READ_ARG, productToRead, productInDB, numOfProductInDB);
		if (everythingOK)
		{
			return 1;
		}

		//if everything went OK, let's write to file
		fileToWrite = fopen(DB_FILE_ARG, "w");
		if (checkFile(fileToWrite))
		{
			return 1;
		}
		sortAndInsertToFile(productInDB, numOfProductInDB, NULL, 0, fileToWrite);
	}

	else
	{
		return printError(WRONG_PARAM);
	}
	if (strcmp(USER_COMMAND, CLEAN))
	{
		fclose(fileToRead);
	}
	fclose(fileToWrite);
	return 0;
}

/**
* check if file is exist and if readable/writeable
*/
int checkFile(FILE *fileToCheck)
{
	if (fileToCheck == NULL)
	{
		return printError(OPEN_FILE_ERROR);
	}
	return 0;

}

/**
* handle clean case
*/
int clean(char *userInput, Product *productToRead, Product *productInDB, int numOfProductInDB)
{
	int i;
	StringProduct temp;
	char *c = strchr(userInput, '-');
	if ((c - userInput) == 0)   //entered a year B.C.
	{
		return printError(WRONG_PARAM);
	}
	temp.year = strtok(userInput, "-");
	temp.month = strtok(NULL, "\n");

	int checkResult = checkingInput(temp, TYPE_CLEAN);
	if (checkResult == ERROR_RETURN)
	{
		return 1;
	}
	else
	{
		productToRead[0].YEAR = atoi(temp.year);
		productToRead[0].MONTH = atoi(temp.month);
	}

	for (i = 0; i < numOfProductInDB; i++)
	{
		if (productInDB[i].YEAR <= productToRead[0].YEAR)
		{
			if (productInDB[i].YEAR == productToRead[0].YEAR)
			{
				if (productInDB[i].MONTH < productToRead[0].MONTH)
				{
					productInDB[i].name[0] = EMPTY_CHAR;
				}
			}
			else
			{
				productInDB[i].name[0] = EMPTY_CHAR;
			}
		}
	}
	return 0;
}

/**
* handle received case
*/
int received(Product *productToRead, Product *productInDB, int numOfProductInDB,
			 int numOfProductReceived)
{
	int receivedIter, DBIter;
	if (numOfProductReceived == ERROR_RETURN)
	{
		return 1;
	}

	//let's check for doubles:
	for (receivedIter = 0; receivedIter < numOfProductReceived; receivedIter++)
	{
		for (DBIter = 0; DBIter < numOfProductInDB; DBIter++)   //loop over product in DB
		{
			if ((!strcmp(productToRead[receivedIter].name, productInDB[DBIter].name)) &&
				(productToRead[receivedIter].barcode == productInDB[DBIter].barcode) &&
				(productToRead[receivedIter].YEAR == productInDB[DBIter].YEAR) &&
				(productToRead[receivedIter].MONTH == productInDB[DBIter].MONTH))
			{
				productInDB[DBIter].weight += productToRead[receivedIter].weight;
				productToRead[receivedIter].name[0] = EMPTY_CHAR;
			}
		}
	}
	return 0;
}

/**
* handle sent case
*/
int sent(Product *productToRead, Product *productInDB, int numOfProductInDB, int numOfProductToSend)
{
	int sentIter, DBIter, numOfAppearenceInDB, minIndex[MAX_LINE_NUMBER], indexIter = -1;
	if (numOfProductToSend == ERROR_RETURN)
	{
		return 1;
	}
	for (sentIter = 0; sentIter < numOfProductToSend; sentIter++) //loop over product to receive
	{
		numOfAppearenceInDB = 0;
		for (DBIter = 0; DBIter < numOfProductInDB; DBIter++)   //loop over product in DB
		{
			if (productToRead[sentIter].barcode == productInDB[DBIter].barcode)
			{
				numOfAppearenceInDB++;

				//if two products with same barcode
				if (numOfAppearenceInDB > 1)
				{
					if (productInDB[DBIter].YEAR < productInDB[minIndex[indexIter]].YEAR)
					{
						indexIter++;
						minIndex[indexIter] = DBIter;
					}
					else if (productInDB[DBIter].YEAR == productInDB[minIndex[indexIter]].YEAR)
					{
						if (productInDB[DBIter].MONTH < productInDB[minIndex[indexIter]].MONTH)
						{
							indexIter++;
							minIndex[indexIter] = DBIter;
						}
					}
				}
					//for the first time that same barcode is found
				else
				{
					indexIter++;
					minIndex[indexIter] = DBIter;
				}
			}
		}
		//we didn't find anything
		if (!numOfAppearenceInDB)
		{
			return printError(NOT_ENOUGH);
		}

		//check if enough products with deviation of 0.001

		if (!indexIter && (productInDB[minIndex[indexIter]].weight - \
			productToRead[sentIter].weight) > -DEVIATION)
		{
			{

				productInDB[minIndex[indexIter]].weight -= productToRead[sentIter].weight;
				//check if product is still exists
				if (productInDB[minIndex[indexIter]].weight <= DEVIATION)
				{
					productInDB[minIndex[indexIter]].name[0] = EMPTY_CHAR;
					numOfProductInDB--;
				}
				indexIter--;
			}

		}
			//if some barcode match, then sent as much as the user want
		else if (indexIter)
		{
			while (indexIter >= 0 && productToRead[sentIter].weight > DEVIATION)
			{
				float temp = productInDB[minIndex[indexIter]].weight;
				productInDB[minIndex[indexIter]].weight -= productToRead[sentIter].weight;
				//check if product is still exists
				if (productInDB[minIndex[indexIter]].weight <= DEVIATION)
				{
					productInDB[minIndex[indexIter]].name[0] = EMPTY_CHAR;
					numOfProductInDB--;
				}
				productToRead[sentIter].weight -= temp;
				indexIter--;
			}
		}
		else
		{
			return printError(NOT_ENOUGH);
		}
	}
	return 0;
}

/**
* get a file and pointer to products array and process the file into the product
*
* @param *fileToRead: pointer to FILE that need to process to struct Product.
* @param *product: pointer to product that data is insert to.
* @return number of product proccessed. -1 in case of an error
*/
int fileToProduct(Product *product, FILE *fileToRead)
{
	int i = 0;
	char line[LINE_LENGTH];
	for (i = 0; i < MAX_LINE_NUMBER && fgets(line, LINE_LENGTH, fileToRead) != NULL; i++)
	{
		StringProduct temp;
		if (strchr(line, '\t') == NULL)
		{
			continue;
		}
		temp.name = strtok(line, "\t");
		temp.barcode = strtok(NULL, "\t");
		temp.weight = strtok(NULL, "\t");
		temp.year = strtok(NULL, "-");
		temp.month = strtok(NULL, "\n");

		//if no weight at all, don't even bother to enter to further process
		if (atof(temp.weight) <= DEVIATION)
		{
			i--;
			continue;
		}

		if (temp.name[0] == '\n')
		{
			break;
		}

		//sent processed product to checking
		int checkResult = checkingInput(temp, TYPE_REGULAR);
		if (checkResult == ERROR_RETURN)
		{
			return ERROR_RETURN;
		}
		else
		{
			strcpy(product[i].name, temp.name);
			sscanf(temp.barcode, "%d", &product[i].barcode);
			sscanf(temp.weight, "%f", &product[i].weight);
			sscanf(temp.year, "%d", &product[i].YEAR);
			sscanf(temp.month, "%d", &product[i].MONTH);
		}
	}
	if (!i)
	{
		printError(UNKNOWN_FORMAT);
		return ERROR_RETURN;
	}
	else
	{
		return i;
	}
}

/**
* same as fileToProduct but a little different
*
* @param *fileToRead: pointer to FILE that need to process to struct Product.
* @param *product: pointer to product that data is insert to.
* @return number of product proccessed. -1 in case of an error
*/
int fileToProductSend(Product *product, FILE *fileToRead)
{
	int i = 0;
	char line[LINE_LENGTH];
	StringProduct temp;
	for (i = 0; i < MAX_LINE_NUMBER && fgets(line, LINE_LENGTH, fileToRead) != NULL; i++)
	{
		temp.barcode = strtok(line, "\t");
		temp.weight = strtok(NULL, "\n");

		int checkResult = checkingInput(temp, TYPE_SENT);
		if (checkResult == ERROR_RETURN)
		{
			return ERROR_RETURN;
		}
		else
		{
			sscanf(temp.barcode, "%d", &product[i].barcode);
			sscanf(temp.weight, "%f", &product[i].weight);
		}
	}
	if (!i)
	{
		printError(UNKNOWN_FORMAT);
		return ERROR_RETURN;
	}
	else
	{
		return i;
	}
}

/**
* checking inputs
*
* @param product: the product we check
* @param type: which struct product we check
* @return 1 in case of an error. 0 if everything is ok
*/
int checkingInput(StringProduct product, int type)
{
	unsigned int i;
	if (type == TYPE_REGULAR)
	{
		//checking name length
		if (strlen(product.name) > 20)
		{
			printError(UNKNOWN_FORMAT);
			return ERROR_RETURN;
		}
	}
	if (type == TYPE_REGULAR || type == TYPE_SENT)
	{
		//checking barcode length and validate
		if (strlen(product.barcode) > 4)
		{
			printError(UNKNOWN_FORMAT);
			return ERROR_RETURN;
		}
		//check if all digit
		for (i = 0; i < strlen(product.barcode); i++)
		{
			if (!isdigit(product.barcode[i]))
			{
				printError(UNKNOWN_FORMAT);
				return ERROR_RETURN;
			}
		}
		//check if all digit
		int commaCount = 0;
		for (i = 0; i < strlen(product.weight); i++)
		{
			if (!isdigit(product.weight[i]) || product.weight[i] == '.')
			{
				//check only 1 comma
				if (product.weight[i] == '.')
				{
					commaCount++;
					if (commaCount == 1)
					{
						continue;
					}
				}
				printError(UNKNOWN_FORMAT);
				return ERROR_RETURN;
			}
		}
	}
	if (type == TYPE_REGULAR || type == TYPE_CLEAN)
	{
		//check if all digit
		for (i = 0; i < strlen(product.year); i++)
		{
			if (!isdigit(product.year[i]) || product.year[i] == '-')
			{
				if (type == TYPE_REGULAR)
				{
					printError(UNKNOWN_FORMAT);
				}
				else
				{
					printError(WRONG_PARAM);
				}
				return ERROR_RETURN;
			}
		}
		if (atoi(product.month) > 12)
		{
			if (type == TYPE_REGULAR)
			{
				printError(UNKNOWN_FORMAT);
			}
			else
			{
				printError(WRONG_PARAM);
			}
			return ERROR_RETURN;
		}
		//check if all digit
		for (i = 0; i < strlen(product.month); i++)
		{
			if (!isdigit(product.month[i]) || product.month[i] == '-')
			{
				if (type == TYPE_REGULAR)
				{
					printError(UNKNOWN_FORMAT);
				}
				else
				{
					printError(WRONG_PARAM);
				}
				return ERROR_RETURN;
			}
		}
	}
	return 0;
}

/**
* gets to array of products, combine them to one, sorting them, and insert to DB file
*
* @param productDB: the products in DB
* @param numOfProductInDB: we need them to iterate over the products
* @param productFILE: the products in other file
* @param numOfProductReceived: we need them to iterate over the products
*/
void sortAndInsertToFile(Product *productDB, int numOfProductInDB, Product *productFILE,
						 int numOfProductReceived, FILE *fileToWrite)
{
	Product productToPrint[2000];
	int numOfProductInDBNEW = numOfProductInDB;
	int numOfProductRecievedNEW = numOfProductReceived;
	int i, j, index = 0;
	for (i = 0, j = 0; i < (numOfProductInDB + numOfProductReceived); i++)
	{
		if (i < numOfProductInDB)
		{
			if (productDB[i].name[0] == EMPTY_CHAR)
			{
				numOfProductInDBNEW--;
				continue;
			}
			productToPrint[index] = productDB[i];
			index++;
		}
		else
		{
			if (productFILE[j].name[0] == EMPTY_CHAR)
			{
				numOfProductRecievedNEW--;
				j++;
				continue;
			}
			productToPrint[index] = productFILE[j];
			j++;
			index++;
		}
	}
	//our own bubble sorting
	for (i = 0; i < (numOfProductInDBNEW + numOfProductRecievedNEW - 1); i++)
	{
		for (j = 0; j < (numOfProductInDBNEW + numOfProductRecievedNEW - i - 1); j++)
		{
			if (productToPrint[j].barcode > productToPrint[j + 1].barcode)
			{
				SWAP
			}
			else if (productToPrint[j].barcode == productToPrint[j + 1].barcode)
			{
				if (productToPrint[j].YEAR >= productToPrint[j + 1].YEAR)
				{
					if (productToPrint[j].YEAR > productToPrint[j + 1].YEAR)   //sort the year
					{
						SWAP
					}
						//if year match, sort the month
					else if (productToPrint[j].MONTH >= productToPrint[j + 1].MONTH)
					{
						if (productToPrint[j].MONTH > productToPrint[j + 1].MONTH)
						{
							SWAP
						}
						else
						{
							if (strcmp(productToPrint[j].name, productToPrint[j + 1].name) > 0)
							{
								SWAP
							}
							else if (strcmp(productToPrint[j].name, productToPrint[j + 1].name) == 0)
							{
								productToPrint[j + 1].weight += productToPrint[j].weight;
								productToPrint[j].name[0] = EMPTY_CHAR;
							}
						}
					}
				}
			}
		}
	}
	//printing to file
	for (i = 0; i < (numOfProductInDBNEW + numOfProductRecievedNEW); i++)
	{
		if (productToPrint[i].name[0] != EMPTY_CHAR)
		{
			fprintf(fileToWrite, "%s\t%d\t%.3f\t%d-%d\n", productToPrint[i].name,
					productToPrint[i].barcode, productToPrint[i].weight, productToPrint[i].YEAR,
					productToPrint[i].MONTH);
		}
	}
}

/**
* all the printing errors in one function
* @param errorType: tells which line to print
*/
int printError(int errorType)
{
	switch (errorType)
	{
		case WRONG_PARAM:
		{
			printf("USAGE: waredb <db file> <command> <command arg file>\n");
			break;
		}
		case OPEN_FILE_ERROR:
		{
			printf("<filename>: no such file\n");
			break;
		}
		case UNKNOWN_FORMAT:
		{
			printf("unknown file format\n");
			break;
		}
		case PERMISSION_ERROR:
		{
			printf("<filename>: Permission denied\n");
			break;
		}
		case NOT_ENOUGH:
		{
			printf("not enough items in warehouse\n");
			break;
		}
		default:
			break;
	}
	return 1;
}


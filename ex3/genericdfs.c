/**
* file that contain the function of generic dfs
*
* @Author - Aviad Levy
* @CS Login - aviadle
*/

#include <stdlib.h>
#include "genericdfs.h"

#define ERROR_DURING_GET_CHILDREN -1  //i assume that if an error happened in the func gerChildren,
//the func return -1

pNode getBest(pNode head, getNodeChildrenFunc getChildren,
		getNodeValFunc getVal, freeNodeFunc freeNode, copyNodeFunc copy, unsigned int best)
{
	pNode bestNode = NULL, *arrChild = NULL;
	int numOfChildren = getChildren(head, &arrChild);
	if (numOfChildren != ERROR_DURING_GET_CHILDREN)   //if no error happened
	{
		int i;
		for (i = 0; i < numOfChildren; i++)
		{
			if (getVal(*arrChild) == best)   //if we found the best
			{
				bestNode = copy(arrChild);
				//we found it, let's free the rest and finish!
				while (*arrChild != NULL)
				{
					freeNode(*arrChild);
				}
				break;
			}
			else if (getVal(*arrChild) > getVal(bestNode))   //found the maximum
			{
				bestNode = copy(*arrChild);
			}
		}
	}
	return bestNode;
}

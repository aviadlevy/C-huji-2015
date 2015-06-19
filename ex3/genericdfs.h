#ifndef GENERICDFS_H
#define GENERICDFS_H

//********      types and functions types
typedef void *pNode;

typedef int getNodeChildrenFunc(pNode, pNode ** /*for the result*/);

typedef unsigned int getNodeValFunc(pNode);

typedef void freeNodeFunc(pNode);

typedef pNode copyNodeFunc(pNode);

/**
* @brief getBest This function returns the node with the highest value in the tree, using
* DFS algorithm.
* @param head The head of the tree.
* @param getChildren A function that gets a node and a pointer to an array of nodes.
* The function allocates memory for an array of all the children of the node, populate it,
* and returns it using the second parameter. The returned value is the number of children.
* @param getVal A function that gets a node and returns its value, as int.
* @param freeNode A function that frees a node from memory. This function will be called for each
* node returned by getChildren.
* @param copy A function that does a deep copy of a node.
* @param best The highest possible value for a node. When the function encounters a node with that
* value, it stops looking and returns it. If the best value can't be determined, pass
* UINT_MAX (defined in limits.h) for that parameter.
* @return The node with the highest value in the tree. In case of an error, or when all the nodes
* in the tree have a value of zero, the returned node is NULL. If some nodes share the best value,
* the function returns the first one it encounters.
*/
pNode getBest(pNode head, getNodeChildrenFunc getChildren,
		getNodeValFunc getVal, freeNodeFunc freeNode, copyNodeFunc copy, unsigned int best);


#endif // GENERICDFS_H

/*---------------------------------------------------------------- 
..Project: dict 3
  radixTree.h :  
         = the interface of module radix tree of the project.
------------------------------------------------------------------*/


#ifndef _RADIXTREE_H_
#define _RADIXTREE_H_
#include "list.h"

/*------------ Constant definitions ------------------------------*/

#define TERMINATING_CHAR "\0"


/*------------ Data definitions ----------------------------------*/

struct treeNode {
    int commonBits;             // Number of bits which are a common prefix to all entries in chain
    char *commonPrefix;         // String containing the entire common prefix
    struct treeNode *branchA;   // Pointer to the left branch - bit following the common prefix is 0
    struct treeNode *branchB;   // Pointer to the right branch - bit following the common prefix is 1
    list_t *dataItems;          // Pointer to a list of associated data items
};
typedef struct treeNode treeNode_t;


/*------------ Function definitions -----------------------------*/
/**
 * Creates and returns a new tree node with relevant information taken in through parameters.
 */
treeNode_t *treeNodeCreate(int bits, char *prefix, treeNode_t *A, treeNode_t *B, list_t *list, place_t *p);

/**
 * Inserts and stores data "p" in a Radix Tree of order 2.
 * @param parent - pointer to parent node of current "root" node.
 * @param root - pointer to current root.
 * @param p - pointer to the data we want to insert.
 * @param prevCommonBits - keeps track of the number of bits already compared in previous nodes.
 * 
 * Returns the updated tree root pointer with the data inserted in the right position.
 */
treeNode_t *treeNodeInsert (treeNode_t *parent, treeNode_t *root, place_t *p, int prevCommonBits);

/**
 * Finds the data element matching "query" by traversing through each node of the Radix Tree.
 * Keeps track of the number of bit, byte and string comparisons while searching.
 * Returns the corresponding list of data items stored at the matching node.
 */
list_t *traverseTree(treeNode_t *root, char *query, int prevCommonBits, int *bitComps, int *byteComps, int *strComps);

/* 
* Frees the tree by recursively freeing child trees first, then freeing the passing parent.
* Also frees the list of data items at each node, if it exists.
*/
void freeTree(treeNode_t *node);

#endif

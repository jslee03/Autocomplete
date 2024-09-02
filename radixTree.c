/*--------------------------------------------------------------- 
..Project: dict3
  radixTree.c :  
          = the implementation of module radix tree of the project.
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "radixTree.h"
#include "data.h"
#include "list.h"
#include "utils.h"

/**
 * Creates and returns a new tree node with relevant information taken in through parameters.
 */
treeNode_t *treeNodeCreate(int bits, char *prefix, treeNode_t *A, treeNode_t *B, list_t *list, 
                           place_t *p) {

    // Allocates memory for the new node
    treeNode_t *treeNode = malloc(sizeof(*treeNode));
    assert(treeNode);
    // Initiate node variables accordingly through paramters
    treeNode->commonBits = bits;
    treeNode->commonPrefix = prefix;
    treeNode->branchA = A;
    treeNode->branchB = B;
    treeNode->dataItems = NULL;

    if (list == NULL && p != NULL) {
        // Adding the first data "p" into linked list "dataItems"
        treeNode->dataItems = listCreate();
        listAppend(treeNode->dataItems, p);
    } else if (list != NULL && p != NULL) {
        // Insert "p" into already created "dataItems" list
        listAppend(treeNode->dataItems, p);
    } else if (list != NULL && p == NULL) {
        // Move an existing "dataItems" list into the list of this node
        treeNode->dataItems = list;
    }

    return treeNode;
}

/**
 * Inserts and stores data "p" in a Radix Tree of order 2.
 * @param parent - pointer to parent node of current "root" node.
 * @param root - pointer to current root.
 * @param p - pointer to the data we want to insert.
 * @param prevCommonBits - keeps track of the number of bits already compared in previous nodes.
 * 
 * Returns the updated tree root pointer with the data inserted in the right position.
 */
treeNode_t *treeNodeInsert (treeNode_t *parent, treeNode_t *root, place_t *p, int prevCommonBits) {
    char *key = p->tradingName; // For convenience, declare "key" as the trading name we are searching for
    int isLeftBranch = 0; // Flag variable

    // Check if current "root" node has a parent node
    if (parent != NULL && parent->branchA == root) {
        isLeftBranch = 1;
    }

    // Insertion of first data "p"
    if (root == NULL) {
        treeNode_t *nullNodeNew = treeNodeCreate(BITS_PER_BYTE, TERMINATING_CHAR, NULL, NULL, NULL, p);
        root = treeNodeCreate(strlen(key)*BITS_PER_BYTE, key, nullNodeNew, NULL, NULL, NULL);
        return root;
    }

    // Find the number of common bits between current root and "key"
    int bits = 0;
    for (int i=prevCommonBits; i<root->commonBits + prevCommonBits; i++) {
        if (bitCmp(root->commonPrefix, key, i) == 0) {
            bits++;
        } else {
            break;
        }
    }
    
    // If there is a full perfect match, including the null terminator of string, 
    // store the data "p" to this current node
    if (prevCommonBits % BITS_PER_BYTE == 0 && strcmp(root->commonPrefix, TERMINATING_CHAR) == 0) {
        listAppend(root->dataItems, p);
        return root;
    }

    // Otherwise, if "key" is not a full perfect match, check if current node "root" is a prefix match
    // CASE 1: prefixes match.
    if (root->commonBits == bits) {
        // Recurse to branch A or branch B, based on the next bit
        prevCommonBits += bits;
        if (getBit(key, prevCommonBits) == 0) {
            // Recurse to branch A if next bit of "key" is a 0
            treeNodeInsert(root, root->branchA, p, prevCommonBits);
        } else {
            // Recurse to branch B if next bit of "key" is a 1
            treeNodeInsert(root, root->branchB, p, prevCommonBits);
        }

    // CASE 2: prefixes do not match, and current "root" is the only node with a common prefix (has no children)
    } else if (root->branchB == NULL) {
        // Insert a new node at bottom of tree
        prevCommonBits += bits;

        // Create a new node that represents the new inserted data
        int newNodeBits = strlen(key)*BITS_PER_BYTE - prevCommonBits;
        treeNode_t *nullNodeNew = treeNodeCreate(BITS_PER_BYTE, TERMINATING_CHAR, NULL, NULL, NULL, p);
        treeNode_t *newNode = treeNodeCreate(newNodeBits, key, nullNodeNew, NULL, NULL, NULL);

        // Create a new node that represents the old root
        treeNode_t *oldRoot;
        if (strcmp(root->commonPrefix, TERMINATING_CHAR) != 0) {
            oldRoot = treeNodeCreate(root->commonBits - bits, root->commonPrefix, root->branchA, root->branchB, NULL, NULL);
        } else {
            oldRoot = treeNodeCreate(root->commonBits - bits, root->commonPrefix, root->branchA, root->branchB, root->dataItems, NULL);
        }

        // Update the new root's common bits and clear the data items
        root->commonBits = bits; 
        root->dataItems = NULL;

        // Determine which branch to follow based on the next bit in the key
        if (getBit(key, prevCommonBits) == 0) {
            root->branchA = newNode;
            root->branchB = oldRoot;
            return root;
        } else {
            root->branchA = oldRoot;
            root->branchB = newNode;
            return root;    
        }

    // CASE 3: prefixes do not match, and current "root" has other nodes with common prefixes (has children)
    } else if (root->branchA != NULL && root->branchB != NULL){
        // Insert a new node at the current position of tree
        prevCommonBits += bits;

        // Create a new node that represents the new inserted data
        int newNodeBits = strlen(key)*BITS_PER_BYTE - prevCommonBits;
        treeNode_t *nullNodeNew = treeNodeCreate(BITS_PER_BYTE, TERMINATING_CHAR, NULL, NULL, NULL, p);
        treeNode_t *newNode = treeNodeCreate(newNodeBits, key, nullNodeNew, NULL, NULL, NULL);

        treeNode_t *newRoot;

        // Determine which branch to follow based on the next bit in the key
        if (getBit(key, prevCommonBits) == 0) {
            // Node with "key" follows branch A
            root->commonBits -= bits; // Change the value of common bits for old root
            newRoot = treeNodeCreate(bits, key, newNode, root, NULL, NULL);
        } else {
            // Node with "key" follows branch B
            root->commonBits -= bits; // Change the value of common bits for old root
            newRoot = treeNodeCreate(bits, key, root, newNode, NULL, NULL);
        }

        // Connect the parent with the new "root"
        if (parent != NULL) {
            if (isLeftBranch) {
                parent->branchA = newRoot;
            } else {
                parent->branchB = newRoot;
            }
        }
        return newRoot;
    }
    return root;
}

/**
 * Finds the data element matching "query" by traversing through each node of the Radix Tree.
 * Keeps track of the number of bit, byte and string comparisons while searching.
 * Returns the corresponding list of data items stored at the matching node.
 */
list_t *traverseTree(treeNode_t *root, char *query, int prevCommonBits, int *bitComps, int *byteComps, int *strComps) {
    int tempBitComps = 0;

    if (root == NULL) {
        return NULL;
    }
    
    // Check if current node is a full string match with the query
    int queryBits = strlen(query) * BITS_PER_BYTE;
    if (prevCommonBits == queryBits && strcmp(root->commonPrefix, TERMINATING_CHAR) == 0) {
        *strComps = 1;
        return root->dataItems;
    }

    // Otherwise, check if there exists a common prefix
    for (int i=prevCommonBits; i<root->commonBits + prevCommonBits; i++) {
        (*bitComps) += 1;
        tempBitComps++;
        // If the prefixes do not match, then this query does not exist within the tree
        if (bitCmp(root->commonPrefix, query, i) != 0) {
            return NULL;
        }
    }

    *byteComps += calcByteComps(tempBitComps);
    prevCommonBits += root->commonBits;

    // If prefixes match, determine which way to branch for traversal
    if (getBit(query, prevCommonBits) == 0) {
        return traverseTree(root->branchA, query, prevCommonBits, bitComps, byteComps, strComps);
    } else {
        return traverseTree(root->branchB, query, prevCommonBits, bitComps, byteComps, strComps);
    }

    return NULL;
}

/* 
* Frees the tree by recursively freeing child trees first, then freeing the passing parent.
* Also frees the list of data items at each node, if it exists.
*/
void freeTree(treeNode_t *node) {
    if (node != NULL) {
        // Recursively free child nodes
        freeTree(node->branchA);
        freeTree(node->branchB);
        // Free list of data items if it exists
        if (node->dataItems != NULL) {
            treeListFree(node->dataItems);
        }
        free(node);
    }
}

/*--------------------------------------------------------------- 
..Project: dict3
  driver3.c : 
             = the main program of the project stage 3

* Created by Jess Lee - 1260948 (jiseungl@student.unimelb.edu.au)
09/09/2023
*
* Radix Tree Autocomplete:
* Stores data about the City of Melbourne in a Radix tree of order 2.
* Accepts trading names, and prints all records matching the prefix.
* The number of bit, byte (character) and string comparisons are also printed.
*
* To run the program type: 
* ./dict3 3 tests/dataset_1000.csv output.txt
*
* Command line input parameters:
*   ./dict3                   Program name
*   3                         Stage number
*   tests/dataset_1000.csv    Input data file
*   output.txt                Output data file
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "list.h"
#include "array.h"
#include "radixTree.h"

/*------------------ Function definitions -----------------------*/
list_t *getPlaceList (FILE *inFile);
treeNode_t *getTree(list_t *placeList);
void treeQuerying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile);

int main(int argc, char **argv) {
    FILE *inFile = fopen(argv[2], "r");
    assert(inFile);
    FILE *outFile = fopen(argv[3], "w");
	assert(outFile);

    treeQuerying(stdin, stdout, inFile, outFile);
	
    fclose(inFile);
    fclose(outFile);

    return 0;
}

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Reads and stores place data from the data input file. 
* Returns linked list of places.
* The following function was adapted from W3.8 of Workshop Week 3.
*/
list_t *getPlaceList (FILE *inFile) {
    // Creates a linked list to store place data
    list_t *places = listCreate();

    placeSkipHeaderLine(inFile);

    // Reads and stores data for each place until the end of the data input file
    place_t *p;
	while ((p = placeRead(inFile))) {
		listAppend(places, p);
	}

    return places;
}

/**
* Reads and stores place data from linked list of places into a Radix Tree of order 2. 
* Returns the root of the tree.
*/
treeNode_t *getTree(list_t *placeList) {
    treeNode_t *root = NULL;
    node_t *curr = placeList->head;
    // Iterate through the linked list "placeList"
    while (curr != NULL) {
        // Insert each data element into the tree
        root = treeNodeInsert(NULL, root, curr->data, 0);
        curr = curr->next;
    }

    return root;
}

/**
* Reads each quey input and searches for matching data stored in the Radix Tree.
* When a match is found, prints the corresponding data information into outFile and stdout.
* The following function was adapted from Assignment 1 (Hyunkyung Kwon & Jess Lee).
*/
void treeQuerying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile) {
    // Builds linked list of places
    list_t *placeList = getPlaceList(inFile);
    // Using this linked list, build a Radix Tree
    treeNode_t *root = getTree(placeList);

    char *query = NULL;
    size_t len = 0;
    list_t *matchList;

    while (getline(&query, &len, queryInput) != -1) {
        // Keeps track of the bit, byte and string comparisons for each query
        int strComps = 0;
        int byteComps = 0;  
        int bitComps = 0;

        // For each quey string, removes the newline character and terminate the string
        int len = strlen(query);
        if (len > 0 && query[len - 1] == '\n') {
            query[len - 1] = '\0';
        }

        matchList = traverseTree(root, query, 0, &bitComps, &byteComps, &strComps);

        // When a match is found, iterate through the list of appropriate data elements
        if (matchList != NULL) {
            node_t *curr = matchList->head;

            fprintf(outFile, "%s\n", query);

            while (curr != NULL) {
                listWrite(curr->data, outFile); // Print each match to outFile
                curr = curr->next;
            }
        }
        fprintf(stdout, "%s --> b%d c%d s%d\n", query, bitComps, byteComps, strComps);
    }

    free(query);
    freeTree(root);
    listFree(placeList);
}

/*--------------------------------------------------------------- 
..Project: dict2
  driver2.c : 
             = the main program of the project stage 2

* Created by Jess Lee - 1260948 (jiseungl@student.unimelb.edu.au)
09/09/2023
*
* Sorted Array Autocomplete:
* Stores data about the City of Melbourne in a sorted array.
* Accepts (partial) trading names, and prints all records matching the prefix.
* The number of bit, byte (character) and string comparisons are also printed.
*
* To run the program type: 
* ./dict2 2 tests/dataset_1000.csv output.txt
*
* Command line input parameters:
*   ./dict2                   Program name
*   2                         Stage number
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

/*------------------ Function definitions -----------------------*/
list_t *getPlaceList (FILE *inFile);
array_t *getPlaceArray (list_t *placeList);
void arrayQuerying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile);


int main(int argc, char **argv) {
    FILE *inFile = fopen(argv[2], "r");
    assert(inFile);
    FILE *outFile = fopen(argv[3], "w");
	assert(outFile);

    arrayQuerying(stdin, stdout, inFile, outFile);
	
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
* Reads and stores place data from linked list of places into a sorted array "placeArr". 
* Returns a sorted array of places.
*/
array_t *getPlaceArray (list_t *placeList) {
    // Creates an array to store sorted place data
    array_t *placeArr = arrayCreate();
    node_t *curr = placeList->head;

    while (curr != NULL) {
        insert(placeArr, curr->data);
        curr = curr->next;
    }

    arrayShrink(placeArr);
    return placeArr;
}

/**
* Reads each quey input and searches for matching data stored in the sorted array "placeArr".
* The following function was adapted from Assignment 1 (Hyunkyung Kwon & Jess Lee).
*/
void arrayQuerying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile) {
    // Builds linked list of places
    list_t *placeList = getPlaceList(inFile);
    // Using this linked list, build a sorted array
    array_t *placeArr = getPlaceArray(placeList);

    char *query = NULL;
    size_t len = 0;

    while (getline(&query, &len, queryInput) != -1) {
        // For each quey string, removes the newline character and terminate the string
        int len = strlen(query);
        if (len > 0 && query[len - 1] == '\n') {
            query[len - 1] = '\0';
        }
        // Finds matching data for each query
        find_and_traverse(placeList, placeArr, query, outFile);
    }

    free(query);
    listFree(placeList);
    arrayFree(placeArr);
}


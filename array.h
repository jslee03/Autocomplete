/*---------------------------------------------------------------- 
..Project: dict2
  array.h :  
         = the interface of module array of the project.
------------------------------------------------------------------*/


#ifndef _ARRAY_H_
#define _ARRAY_H_
#include <stdio.h>
#include "data.h"
#include "list.h"
#include "utils.h"

/*------------ Constant definitions ------------------------------*/

#define INIT_SIZE 2


/*------------ Data definitions ----------------------------------*/

struct array {
	place_t **A;        // Use as array of place_t *, ie. place_t *A[]
	int size;           // Current array size
	int n;              // Current number of elements used
};
typedef struct array array_t;


/*------------ Function definitions -----------------------------*/
/**
* Creates and returns an empty array.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
array_t *arrayCreate();

/**
* Ensures that the array's size is large enough to add more elements.
* If not, create more space.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
void arrayEnsureSize(array_t *arr);

/**
* Inserts data "p" into sorted array "arr", ensuring "arr" remain sorted.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
void insert(array_t *arr, place_t *p);

/**
* Function copied from W3.6 of Workshop Week 3.
* Shrinks the array to the same size as the number of elements used.
*/
void arrayShrink(array_t *arr);

/**
 * Finds a data element matching "prefix" by initially using binary search.
 * After that, linearly searches around this location to find adjacent values which also match the "prefix".
 */
void find_and_traverse(list_t *list, array_t *arr, char *prefix, FILE *outFile);

/**
* Binary searches and returns the index of place_t data in "arr" that has the same 
* trading name as "prefix".
* Keeps track of the number of byte and string comparisons while searching.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
int binarySearch(FILE *outFile, array_t *arr, char *prefix, int *byteComps, int *strComps);

/**
* Linearly searches around index "index" for elements in "arr" that has the same trading 
* name as "prefix", and keeps track of the number of byte and string comparisons while searching.
* When a match is found, writes information for the matching data to the output file.
*/
void linearSearch(list_t *list, FILE *outFile, array_t *arr, char *prefix, int index, int *byteComps, int *strComps);

/**
 * Frees memory used by array "arr".
 */
void arrayFree(array_t *arr);

#endif

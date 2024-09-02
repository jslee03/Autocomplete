/*--------------------------------------------------------------- 
..Project: dict2
  array.c :  
         = the implementation of module array of the project.
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "array.h"
#include "data.h"
#include "utils.h"

/**
* Creates and returns an empty array.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
array_t *arrayCreate() {
    // Allocates memory for the new array
    array_t *arr = malloc(sizeof(*arr));
    assert(arr);
    // Initiates struct variables
    arr->size = INIT_SIZE;
    arr->A = malloc(INIT_SIZE * sizeof(*(arr->A)));
    assert(arr->A);
    arr->n = 0;
    return arr;
}

/**
* Ensures that the array's size is large enough to add more elements.
* If not, create more space.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
void arrayEnsureSize(array_t *arr) {
	if (arr->n == arr->size) {
		arr->size *= 2;
		arr->A = realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

/**
* Inserts data "p" into sorted array "arr", ensuring "arr" remain sorted.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
void insert(array_t *arr, place_t *p) {
    int i = arr->n - 1;
    arrayEnsureSize(arr);

    // Starting from the end of the array, shift all elements one position to the right
    while (i >= 0 && strcmp(arr->A[i]->tradingName, p->tradingName) > 0) {
        arr->A[i+1] = arr->A[i];
        i--;
    }
    // Assign new data "p" to the appropriate position in array
    arr->A[i+1] = p;
    arr->n++;
}

/**
* Function copied from W3.6 of Workshop Week 3.
* Shrinks the array to the same size as the number of elements used.
*/
void arrayShrink(array_t *arr) {
	if (arr->size != arr->n) {
		// The new size should be at least INIT_SIZE
		arr->size = (arr->n > INIT_SIZE) ? arr->n : INIT_SIZE;
		arr->A = realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

/**
 * Finds a data element matching "prefix" by initially using binary search.
 * After that, linearly searches around this location to find adjacent values which also match the "prefix".
 */
void find_and_traverse(list_t *list, array_t *arr, char *prefix, FILE *outFile) {
    // Declaration of variables that keep track of the bit, byte and string 
    // comparisons for each prefix search
    int strComps = 0;
    int byteComps = 0;  
    int bitComps;

    // Find "index" through binary search
    int index = binarySearch(outFile, arr, prefix, &byteComps, &strComps);

    // Perform linear search around this "index"
    linearSearch(list, outFile, arr, prefix, index, &byteComps, &strComps);

    bitComps = byteComps * 8;
    // Print corresponding bit, byte and string comparison number for each query input to stdout
    fprintf(stdout, "%s --> b%d c%d s%d\n", prefix, bitComps, byteComps, strComps);
}

/**
* Binary searches and returns the index of place_t data in "arr" that has the same 
* trading name as "prefix".
* Keeps track of the number of byte and string comparisons while searching.
* The following function was adapted from W3.6 of Workshop Week 3.
*/
int binarySearch(FILE *outFile, array_t *arr, char *prefix, int *byteComps, int *strComps) {
    int lo = 0;
    int hi = arr->n - 1;
    int cmpResult;

    while (lo <= hi) {
        int mid = (lo+hi)/2;
        (*strComps)++;
        cmpResult = nameCmp(prefix, getTradingName(arr->A[mid]), byteComps);

        if (cmpResult == 0) {
            lo = mid;
            break;
        } else if (cmpResult < 0) {
            // Move search place to the left
            hi = mid - 1;
        } else if (cmpResult >0) {
            // Move search place to the right
            lo = mid + 1;
        }
    }
    return lo;
}

/**
* Linearly searches around index "index" for elements in "arr" that has the same trading 
* name as "prefix", and keeps track of the number of byte and string comparisons while searching.
* When a match is found, writes information for the matching data to the output file.
*/
void linearSearch(list_t *list, FILE *outFile, array_t *arr, char *prefix, int index, 
                  int *byteComps, int *strComps) {

    char *name = getTradingName(arr->A[index]);
    int maxSize = 2;
    // Creates a temporary array to store matching elements to the left 
    // of the index found in binary search
    place_t *tempArr = malloc(maxSize * sizeof(place_t));
    int numElem = 0;
    
    fprintf(outFile, "%s\n", prefix);

    /* --------------- Linearly search to the left of the index point --------------- */
    int prevName = index - 1;
    int stop = 0; // Flag variable
    while (prevName >= 0) {
        int j = 0;
        name = getTradingName(arr->A[prevName]);

        if (stop) {
            break;
        }

        (*strComps)++;

        while (1) {
            // Compare each character in the name of data element in "arr" and searching "prefix"
            if (prefix[j] == name[j]) {
                (*byteComps)++;
                j++;
                if (prefix[j] == '\0') {
                    // If it is a full match, ensure there is enough space in temporary array...
                    if (numElem >= maxSize) {
                        maxSize *= 2;
                        tempArr = realloc(tempArr, sizeof(place_t) * maxSize);
                    } 
                    // ... and add the found data element into temporary array
                    tempArr[numElem++] = *(arr->A[prevName]);
                    
                    (*byteComps)++;
                    // Break out of loop to compare the next left element in "arr"
                    prevName--;
                    break;
                }
            }
            else {
                // If there is no match, no need to keep searching left as "arr" is sorted
                (*byteComps)++;
                stop = 1; // Set flag variable to stop searching left of "arr"
                break;
            }
        }
    }
    /* --------------------------- End left linear search ---------------------------- */


    // Write information to the output file in reverse order for all matching data stored in "tempArr"
    for (int i = numElem-1; i >= 0; i--) {
        listWrite(&(tempArr[i]), outFile);
    }
    free(tempArr);
    

    // Write information for the data element at "index" to the output file
    listWrite(arr->A[index], outFile);


    /* --------------- Linearly search to the right of the index point --------------- */
    int nextName = index + 1;
    stop = 0; // Flag variable
    while (nextName < arr->n) {
        name = getTradingName(arr->A[nextName]);
        int j = 0;

        if (stop) {
            break;
        }

        (*strComps)++;

        while (1) {
            // Compare each character in the name of data element in "arr" and searching "prefix"
            if (prefix[j] == name[j]) {
                (*byteComps)++;
                j++;
                if (prefix[j] == '\0') {
                    // If it is a full match, write information for this data element to the output file
                    listWrite(arr->A[nextName], outFile);
                    (*byteComps)++;
                    // Break out of loop to compare the next right element in "arr"
                    nextName++;
                    break;
                }
            }
            else {
                // If there is no match, no need to keep searching right as "arr" is sorted
                (*byteComps)++;
                stop = 1; // Set flag variable to stop searching right of "arr"
                break;
            }
        }
    }
    /* --------------------------- End right linear search --------------------------- */
}

/**
 * Frees memory used by array "arr".
 */
void arrayFree(array_t *arr) {
    free(arr->A);
    free(arr);
}


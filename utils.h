/*---------------------------------------------------------------- 
..Project: dict2, dict3
  utils.h :  
         = the interface of module utility of the project.
------------------------------------------------------------------*/


#ifndef _UTILS_H_
#define _UTILS_H_

/*------------ Constant definitions ------------------------------*/

#define BITS_PER_BYTE 8


/*------------ Function definitions -----------------------------*/
/**
 * An adaptation of the strcmp function in string.h header file.
 * Increments the number of byte comparisons per character compared.
 * Returns 0 if two strings are identical.
 */
int nameCmp(char *prefix, char *name, int *byteComps);

/**
 * Returns a particular bit from a given string and bit positon.
 */
int getBit(char *s, int n);

/**
 * Compares the bit of two strings at a particular bit position.
 * Returns 0 if the bits are identical.
 */
int bitCmp(char *a, char *b, int pos);

/**
 * Calculates and returns the number of byte comparisons made according to the bit comparisons.
 */
int calcByteComps(int bits);

#endif

/*---------------------------------------------------------------- 
..Project: dict2, dict3
  utils.c :  
         = the implementation of module utility of the project.
------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * Returns a particular bit from a given string and bit positon.
 */
int getBit(char *s, int n) {
    int pos = n / BITS_PER_BYTE; // Index position of the character in string "s"
    char byte = s[pos]; // Character we are interested in

    pos = n % BITS_PER_BYTE; // Number of bit positions required to shift
    
    byte = byte << pos;
    byte = byte >> 7;

    return byte;
}

/**
 * Compares the bit of two strings at a particular bit position.
 * Returns 0 if the bits are identical.
 */
int bitCmp(char *a, char *b, int pos) {
    // Get the bit for each string "a" and "b" at position "pos".
    int a_bit = getBit(a, pos);
    int b_bit = getBit(b, pos);

    if (a_bit == b_bit) {
        return 0;
    } else if (a_bit > b_bit) {
        return 1;
    } else {
        return -1;
    }
}

/**
 * Calculates and returns the number of byte comparisons made according to the bit comparisons.
 */
int calcByteComps(int bits) {
    int byteComps = 0;

    if (bits/BITS_PER_BYTE == 0) {
        byteComps++;
    } else if (bits%BITS_PER_BYTE == 0) {
        byteComps += bits/BITS_PER_BYTE;
    } else {
        byteComps += (bits/BITS_PER_BYTE + 1);
    }
    
    return byteComps;
}

/**
 * An adaptation of the strcmp function in string.h header file.
 * Increments the number of byte comparisons per character compared.
 * Returns 0 if two strings are identical.
 */
int nameCmp(char *prefix, char *name, int *byteComps) {
    int len = strlen(prefix);

    for (int i=0; i<len; i++) {
        (*byteComps)++;
        if (prefix[i] > name[i]) {
            return 1;
        } else if (prefix[i] < name[i]) {
            return -1;
        }
        if (i+1 == len) {
            (*byteComps)++; // Include the count for the null terminator
            return 0;
        }
    }
    return 0;
}

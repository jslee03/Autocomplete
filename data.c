/*--------------------------------------------------------------- 
..Project: dict1
  data.c :  
         = the implementation of module data of the project.
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "data.h"

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Reads a single line from data input file "f" to build place_t data.
* Returns the pointer, or NULL if reading is unsuccessful.
*/
place_t *placeRead (FILE *f) {
    char *line = NULL;
    size_t len = 0;
    bool separateAtComma = true;
    bool openingQuote = true;
    place_t *p = NULL; // Declare an instance of the data struct

    if (getline(&line, &len, f) != -1) {
        int indexL = 0; // Keeps track of index of each line
        int indexF = 0; // Keeps track of index of each field 
        int fieldNum = 0; // Keeps track of the type of field

        // Allocate memory for the data struct p
        p = malloc(sizeof(*p));
		assert(p);

        // Allocate memory for a single field
        char *field = malloc(MAX_CHAR_LEN);
        assert(field);
        
        // Iterate through each character until end of line
        while (line[indexL] != '\0' && line[indexL] != '\n') {
            field[indexF] = '\0';

            // Skip the opening quotatation mark and do not separate each field at comma
            if ((line[indexL] == DOUBLE_QUOTE) && openingQuote) {
                separateAtComma = false;
                openingQuote = false;
                // Go to next character in the line
                indexL++;
                continue;
            }
            
            // At comma, assign the field to data struct
            if ((line[indexL] == ',' && separateAtComma) || line[indexL+1] == '\0') {
                assignField(p, field, fieldNum);
                // Free field memory for allocation of a new field
                free(field);
                field = calloc(MAX_CHAR_LEN, sizeof(*field));
                indexF = 0;
                fieldNum++;
                // Go to next character in the line
                indexL++;
                continue;
            } else {
                // Assign the field to data struct when closing quotation mark is read
                if (line[indexL] == DOUBLE_QUOTE) {
                    assignField(p, field, fieldNum);
                    separateAtComma = true;
                    openingQuote = true;
                    // Free field memory for allocation of a new field
                    free(field);
                    field = calloc(MAX_CHAR_LEN, sizeof(*field));
                    fieldNum++;
                    indexF = 0;
                    // Skip a character (comma), and go to the next character in the line
                    indexL += 2;
                    continue;
                }
                // Build field string
                field[indexF] = line[indexL];
                field[indexF + 1] = '\0';
                // Go to next character in the line
                indexF++;
                indexL++;    
            }
        }
        // Assign the last field to data struct
        assignField(p, field, fieldNum);
        free(field);
    }
    free(line);
    return p;
}

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Assigns string to its corresponding field in data struct.
*/
void assignField (place_t *p, char *field, int fieldNum) {
    switch(fieldNum) {
        case CENSUS_YEAR:
            p->censusYear = atoi(field);
            break;
        case BLOCK_ID:
            p->blockId = atoi(field);
            break;
        case PROPERTY_ID:
            p->propertyId = atoi(field);
            break;
        case BASE_PROPERTY_ID:
            p->basePropertyId = atoi(field);
            break;
        case BUILDING_ADDRESS:
            p->buildingAddress = strdup(field);
            assert(p->buildingAddress);
            break;
        case CLUE_SMALL_AREA:
            p->clueSmallArea = strdup(field);
            assert(p->clueSmallArea);
            break;
        case BUSINESS_ADDRESS:
            p->businessAddress = strdup(field);
            assert(p->businessAddress);
            break;
        case TRADING_NAME:
            p->tradingName = strdup(field);
            assert(p->tradingName);
            break;
        case INDUSTRY_CODE:
            p->industryCode = atoi(field);
            break;
        case INDUSTRY_DESCRIPTION:
            p->industryDescription = strdup(field);
            assert(p->industryDescription);
            break;
        case SEATING_TYPE:
            p->seatingType = strdup(field);
            assert(p->seatingType);
            break;
        case NUMBER_OF_SEATS:
            p->numberOfSeats = atoi(field);
            break;
        case LONGITUDE:
            p->longitude = strtod(field, NULL);
            break;
        case LATITUDE:
            p->latitude = strtod(field, NULL);
            break;
    } 
}

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Skips the header line of data input file.
*/
void placeSkipHeaderLine(FILE *f) {
    char *line = NULL;
    size_t len = 0;

    if (getline(&line, &len, f) != -1) {};

    free(line);
}

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Frees the memory allocated to each field element and the data struct itself. 
*/
void placeFree (place_t *p) {
    assert(p != NULL);
    free(p->buildingAddress);
    free(p->clueSmallArea);
    free(p->businessAddress);
    free(p->tradingName);
    free(p->industryDescription);
    free(p->seatingType);
    free(p);
}

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Writes information for the matching data to the output file.
*/
void listWrite (place_t *match, FILE *outFile) {
	fprintf(outFile, "--> census_year: %d || block_id: %d || property_id: %d || "
					"base_property_id: %d || building_address: %s || clue_small_area: %s || "
					"business_address: %s || trading_name: %s || industry_code: %d || "
					"industry_description: %s || seating_type: %s || number_of_seats: %d || "
					"longitude: %.5lf || latitude: %.5lf || \n", 
	match->censusYear, 
	match->blockId, 
	match->propertyId, 
	match->basePropertyId, 
	match->buildingAddress, 
	match->clueSmallArea, 
	match->businessAddress, 
	match->tradingName, 
	match->industryCode, 
	match->industryDescription, 
	match->seatingType, 
	match->numberOfSeats, 
	match->longitude, 
	match->latitude);
}

/**
 * Returns the trading name of the place_t data input.
 */
char *getTradingName(place_t *p) {
    assert(p);
    return p->tradingName;
}

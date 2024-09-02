/*---------------------------------------------------------------- 
..Project: dict2, dict3
  data.h :  
         = the interface of module data of the project.
------------------------------------------------------------------*/


#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>

/*------------ Constant definitions ------------------------------*/

#define MAX_CHAR_LEN 128
#define MAX_LINE_LEN 512
#define DOUBLE_QUOTE '\"'

#define CENSUS_YEAR 0
#define BLOCK_ID 1
#define PROPERTY_ID 2
#define BASE_PROPERTY_ID 3
#define BUILDING_ADDRESS 4
#define CLUE_SMALL_AREA 5
#define BUSINESS_ADDRESS 6
#define TRADING_NAME 7
#define INDUSTRY_CODE 8
#define INDUSTRY_DESCRIPTION 9
#define SEATING_TYPE 10
#define NUMBER_OF_SEATS 11
#define LONGITUDE 12
#define LATITUDE 13


/*------------ Data definitions ----------------------------------*/

struct place {
    int censusYear;                 // The year the information was recorded for. (integer)
    int blockId;                    // The city block ID. (integer)
    int propertyId;                 // The ID of the property. (integer)
    int basePropertyId;             // The ID of the building the business is in. (integer)
    char *buildingAddress;          // The address of the building. (string)
    char *clueSmallArea;            // The CLUE area of Melbourne that the building is in. (string)
    char *businessAddress;          // The address of the business itself. (string)
    char *tradingName;              // The name of the business. (string)
    int industryCode;               // The ID for the category of the business. (integer)
    char *industryDescription;      // The description of the category of the business. (string)
    char *seatingType;              // The type of seating the record describes. (string)
    int numberOfSeats;              // The number of seats provided of this type. (integer)
    double longitude;               // The longitude (x) of the seating location. (double)
    double latitude;                // The latitude (y) of the seating location. (double)
};
typedef struct place place_t;


/*------------ Function definitions -----------------------------*/
/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Reads a single line from data input file "f" to build place_t data.
* Returns the pointer, or NULL if reading is unsuccessful.
*/
place_t *placeRead(FILE *f);

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Assigns string to its corresponding field in data struct.
*/
void assignField(place_t *p, char *field, int fieldNum);

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Skips the header line of data input file.
*/
void placeSkipHeaderLine(FILE *f);

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Frees the memory allocated to each field element and the data struct itself. 
*/
void placeFree (place_t *p);

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Writes information for the matching data to the output file.
*/
void listWrite (place_t *match, FILE *outFile);

/**
 * Returns the trading name of the place_t data input.
 */
char *getTradingName(place_t *p);

#endif

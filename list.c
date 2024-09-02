/*--------------------------------------------------------------- 
..Project: dict2, dict3
  list.c :  
          = the implementation of module linkedList of the project.
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "data.h"

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Creates and returns an empty linked list.
* The following function was adapted from W3.8 of Workshop Week 3.
*/
list_t *listCreate() {
	// Allocates memory for the new list
	list_t *list = malloc(sizeof(*list));
	assert(list);
	// Initiates struct variables
	list->head = list->tail = NULL;
	list->n = 0;
	return list;
}

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Inserts a new node with place "data" to the end of "list". 
* The following function was adapted from W3.8 of Workshop Week 3.
*/
void listAppend(list_t *list, place_t *data) {
	assert(list);

	// Creates a new node and allocates memory
	// Sets data to the new node
	node_t *new = malloc(sizeof(*new));
	assert(new);
	new->data = data;
	new->next = NULL;

	// If list is empty, sets the new node to both head and tail
	if (list->tail == NULL){
		list->head = list->tail = new;
	}
	// If list is not empty, adds the data to the end of the list
	else {
    	list->tail->next = new;
    	list->tail = new;
	}
	
	// Increments the number of list elements by 1
	(list->n++);
}

/**
* Function copied from Assignment 1 (Hyunkyung Kwon & Jess Lee).
* Free the memory allocated to list elements and the list itself. 
*/
void listFree(list_t *list) {
	assert(list != NULL);
	node_t *curr, *tmp;
	curr = list->head;

	while (curr) {
		// Free the elements of each data first
		placeFree(curr->data);
		tmp = curr;
		curr = curr->next;
		// Free each node of the linked list
		free(tmp);
	}
	// Free the list itself
	free(list);
}

/**
* Frees the list "dataItems" in each node of the tree, if it exists. 
* The following function was adapted from Assignment 1 (Hyunkyung Kwon & Jess Lee).
*/
void treeListFree(list_t *list) {
	assert(list != NULL);
	node_t *curr, *tmp;
	curr = list->head;

	while (curr) {
		tmp = curr;
		curr = curr->next;
		// Free each node of the linked list
		free(tmp);
	}
	// Free the list itself
	free(list);
}

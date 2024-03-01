// set.c, Grant Johnson, COEN 12, 11/2/2023
// This program stores unique data from a file into an array of lists using a hash table and linked lists. The provided functions are used by provided files to access/manipulate the stored  data type. The data type is determined by the user

#include "list.h"
#include "set.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct set {
	int length;
	int count;
	LIST **lists;
	int (*compare)();
	unsigned (*hash)();
} SET; 

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {		// Creates set for other functions
  int i;									// O(n), with n being max number of lists in the "lists" array
	SET *sp;
	assert(compare != NULL && hash != NULL);
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->length = maxElts;
	sp->compare = compare;
	sp->hash = hash;
	sp->count = 0;
	sp->lists = malloc(sizeof(LIST*) * maxElts);
	assert (sp != NULL);
	for (i = 0; i < sp->length; i++) {
		sp->lists[i] = createList(sp->compare);
	}
	return sp;
	}

void destroySet(SET *sp) {							// Destroys pointers in each set, frees sp, and frees lists using destroyList function
	assert((sp != NULL));	       	      			 		// O(n), with n being max number of lists in the "lists" array
	int i;
	
	for(i = 0; i < sp->length; i++) {
		destroyList(sp->lists[i]);
	}
	free(sp->lists);
        free(sp);
}

int numElements(SET *sp) {							// Returns number of elements in sp
	return sp->count;							// O(1), since number of elements is an existing constant
}

void addElement(SET *sp, void *elt) {					       	// Adds element to list using list index provided from hash table
	assert((sp != NULL) && (elt != NULL));				       	// O(1), since process of adding has a fixed runtime complexity
	int index;
	index = (sp->hash)(elt) % sp->length;
	
	if (findItem(sp->lists[index], elt) == NULL) {
		sp->count = sp->count + 1;
		addLast(sp->lists[index], elt);
	}
}

void removeElement(SET *sp, void *elt) {					// Removes element from list using list index provided from hash table
  	assert((sp != NULL) && (elt != NULL));					// O(m), where m is number of items in list
	int index;
	index = (sp->hash)(elt) % sp->length;

	if (findItem(sp->lists[index], elt) != NULL) {
		removeItem(sp->lists[index], elt);
                sp->count = sp->count - 1;
	}
}

void *findElement(SET *sp, void *elt) {						 // Finds element in list using index from hash table or returns NULL if it is not present
	assert((sp != NULL) && (elt != NULL));					 // O(m), where m is number of items in list
	void *item;
        item = findItem(sp->lists[(sp->hash)(elt) % sp->length], elt);
	if (item != NULL) {
		return item;
	}
	return NULL;
}

void *getElements(SET *sp) {							// Provides unified list of items from all lists in set
	assert(sp != NULL);							// O(n), where n is the number of lists in "lists" array
	int i;
	int j;
	i = 0;
	j = 0;
	void **tp;
	tp = malloc(sizeof(void*) * sp->count);
	assert(tp != NULL);	

        for(i = 0; i < sp->length; i++) {
        	memcpy(tp+j, getItems(sp->lists[i]), numItems(sp->lists[i]) * sizeof(void*));
		j = j + numItems(sp->lists[i]);
	}
	return tp;
}

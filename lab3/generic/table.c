//table.C, Grant Johnson, COEN 12, 10/18/2023
//This program stores unique data from a text into an array using a hash table and provided functions to access/manipulate the stored  words. The data type is determined by the user

#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct set {
	int len;
	int count;
	void **data;
	char *flags;
	int (*compare)();
	unsigned (*hash)();
} SET; 

SET *createSet(int maxElts, int (*compare)(), unsigned(*hash)()) {		// Creates set for other functions
	assert ((compare != NULL) && (hash != NULL));				// O(n), with n being max number of elements in flags array
	int i;
	SET *sp;
 	sp = malloc(sizeof(SET));
	assert (sp != NULL);
	sp -> len = maxElts;
	sp -> count = 0;
	sp -> data = malloc(sizeof(void*) * maxElts);
	assert (sp != NULL);
	sp -> flags = malloc(sizeof(char) * maxElts);
	assert (sp != NULL);
	sp->compare = compare;
	sp->hash = hash;

	for (i = 0; i < maxElts; i++) {
		sp->flags[i] = 'E';
	}

	return sp;
}

static int search(SET *sp, void *elt, bool *found) {			      // Searches if inputed element exists in array and provided index to add element to array if it is not found
        assert((sp != NULL) && (elt != NULL) && (found != NULL));             // O(n), with n being the total number of elements in the array if the array is full during probing
        int i;
	int hashIndex;
	int deletedIndex;
	int startIndex;
	deletedIndex = -1;
	startIndex = (sp->hash)(elt) % sp->len;
	*found = false;

        for (i = 0; i < sp->len; i++) {
		hashIndex = (startIndex + i) % sp->len;
                if (sp->flags[hashIndex] == 'E') {
                	if (deletedIndex != -1) {
	                        return deletedIndex;
                	}
		        return hashIndex;
		}
		if (sp->flags[hashIndex] == 'D') {
                        if (deletedIndex == -1) {
                                deletedIndex = hashIndex;
                        }
                }
		if ((sp->compare)(sp->data[hashIndex], elt) == 0) {
                        *found = true;
			return hashIndex;
                }
	}
	return -1;
}

void destroySet(SET *sp) {							// Deletes flags array and sp array, and free their allocated memory
	assert(sp != NULL);							// O(1), constant runtime completity
	free(sp->flags);
	free(sp->data);
	free(sp);
}

int numElements(SET *sp) {							// Returns # of elements in array
	assert(sp != NULL);							// O(1), constant runtime completity
	return sp->count;
}

void addElement(SET *sp, void *elt) {						// Adds inputed element pointer to array using search function to find index
        int index;								// O(n), since runtime complexity is constant, but calls search which has O(n) runtime
	bool found;
	assert((sp != NULL) && (elt != NULL));

	index = search(sp, elt, &found);
	if (!found) {
		assert(sp->count < sp->len);
		sp->data[index] = elt;
                sp->count = sp->count + 1;               
		sp->flags[index] = 'F';
	}
}

void removeElement(SET *sp, void *elt) {					// Lists inputed element as deleted
	bool found;								// O(n), since runtime complexity is constant, but calls search which has O(n) runtime
	int index;
	assert((sp != NULL) && (elt != NULL));
	
        index = search(sp, elt, &found);
        if (found == true) {
		sp->flags[index] = 'D';
		sp->count = (sp->count - 1);
	}
}

void *findElement(SET *sp, void *elt) {						// Finds inputed element in array
	assert((sp != NULL) && (elt != NULL));					// O(n), since runtime complexity is constant, but calls search which has O(n) runtime
	bool found;
	int index;

	index = search(sp, elt, &found);
	if (found == true) {
		return sp->data[index];
	}
	return NULL;
}

void *getElements(SET *sp) {							// Returns new array tp of all elements in sp array
	assert(sp != NULL);							// O(n), with n being the total number of elements in the array
	void **tp;
	int i;
	int k;
	k = 0;
	tp = malloc(sizeof(void*) * sp->count);
	assert(tp != NULL);	
	
	for(i = 0; i < sp->len; i++) {
		if (sp->flags[i] == 'F') {
			tp[k] = sp->data[i];
			k = k + 1;
		}
	}
	
	return tp;
}

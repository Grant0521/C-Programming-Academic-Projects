//Grant Johnson, COEN 12, 10/4/2023
//This program makes a sorted array of unique words from a text

#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct set {
	int len;
	int count;
	char **data;
} SET; 

SET *createSet(int maxElts) {				// Creates set for other functions, O(1)
	SET *sp;
 	sp = malloc(sizeof(SET));
	assert (sp != NULL);
	sp -> len = maxElts;
	sp -> count = 0;
	sp -> data = malloc(sizeof(char*) * maxElts);
	assert (sp != NULL);

	return sp;
}

static int search(SET *sp, char *elt, bool *found) {			// Binary search for value, log(n)
	assert(sp != NULL);
	int low;
	int high;
	int mid;

	*found = false;
	
	low = 0;
	high = sp->count - 1;
	
	while(low <= high) {
		mid = (low + high) / 2;
		if (strcmp(sp->data[mid], elt) == 0) {
			*found = true;
			return mid;
		}
		else if (strcmp(sp->data[mid], elt) > 0) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	return low;
}

void destroySet(SET *sp) {				// Frees memory from array, O(n)
	assert(sp != NULL);
	int i;
	for (i = 0; i <	sp->count; i++) {
		free(sp->data[i]);
	}
	free(sp->data);
	free(sp);
}

int numElements(SET *sp) {				// Finds # of elements in array, O(1)
	assert(sp != NULL);
	return sp->count;
}

void addElement(SET *sp, char *elt) {			// Adds element to array, O(n)
	assert(sp != NULL);
	assert(elt != NULL);
        int index;
	int i;
	bool found;
	
	found = false;	
	index = search(sp, elt, &found);

	if (found == false) {
		for (i=sp->count; i > index; i--) {
			sp->data[i] = strdup(sp->data[i-1]);
		}
		sp->data[index] = strdup(elt);
		sp->count = sp->count + 1;
        }
}

void removeElement(SET *sp, char *elt) {		// Removes element from array, O(n)
	assert(sp != NULL);
	assert(elt != NULL);
	int index;
	int i;
        bool found;

	found = false;
        index = search(sp, elt, &found);

        if (found == true) {
		free(sp->data[index]);
                for (i=index + 1; i < sp->count; i++) {
                        sp->data[i-1] = sp->data[i];
                }
		sp->count = (sp->count - 1);
	}
}

char *findElement(SET *sp, char *elt) {			// Finds element in array, O(1)
	assert(sp != NULL);
	assert(elt != NULL);
	int index;
	bool found;

	index = search(sp, elt, &found);
	return found ? sp->data[index] : NULL;
}

char **getElements(SET *sp) {				// Gets all elements in array, O(1)
	assert(sp != NULL);
	char **tp;
	tp = malloc(sizeof(char*) * sp->count);
	assert(tp != NULL);	
	memcpy(tp, sp->data, sizeof(char*) * sp->count);
	return tp;
}

//Grant Johnson, COEN 12, 9/27/2023
////This program makes an unsorted array of unique words from a text

#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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

static int search(SET *sp, char *elt) {			// Linear search for value, O(n)
        assert(sp != NULL);
        int i;
        for (i = 0; i < sp->count; i++) {
                if (strcmp(sp->data[i], elt) == 0) {
                        return i;
                }
        }
	return -1;
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

void addElement(SET *sp, char *elt) {			// Adds element to array, O(1)
	assert(sp != NULL);
	assert(elt != NULL);
        int index;
	
	index = search(sp, elt);
	if (index == -1) {
		sp->data[sp->count] = strdup(elt);
		sp->count = sp->count + 1;
        }
}

void removeElement(SET *sp, char *elt) {		// Removes element from array, O(1)
	int index;
	assert(sp != NULL);
	assert(elt != NULL);
	index = search(sp, elt);
	
	if (index != -1) {
		free(sp->data[index]);
		sp->count = (sp->count - 1);
		sp->data[index] = sp->data[sp->count];
	}
}

char *findElement(SET *sp, char *elt) {			// Finds element in array, O(1)
	assert(sp != NULL);
	assert(elt != NULL);
	int index;
	index = search(sp, elt);
	if (index != -1) {
		return sp->data[index];
	}
	return NULL;
}

char **getElements(SET *sp) {				// Gets all elements in array, O(1)
	assert(sp != NULL);
	char **tp;
	tp = malloc(sizeof(char*) * sp->count);
	assert(tp != NULL);	
	memcpy(tp, sp->data, sizeof(char*) * sp->count);
	return tp;
}


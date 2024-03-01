//Table.C, Grant Johnson, COEN 12, 10/11/2023
//This program stores unique words from a text into an array using a hash table and provided functions to access/manipulate the stored  words

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
	char *flags;
} SET; 

SET *createSet(int maxElts) {							// Creates set for other functions
	int i;									// O(n), with n being max number of elements in flags array
	SET *sp;
 	sp = malloc(sizeof(SET));
	assert (sp != NULL);
	sp -> len = maxElts;
	sp -> count = 0;
	sp -> data = malloc(sizeof(char*) * maxElts);
	assert (sp != NULL);
	sp -> flags = malloc(sizeof(char) * maxElts);
	assert (sp != NULL);

	for (i = 0; i < maxElts; i++) {
		sp->flags[i] = 'E';
	}

	return sp;
}

unsigned strhash(char *s) {                                                   // Provides hash table index value based on provided string
        assert(s != NULL);						      // O(s), with s being the length of the string
        unsigned hash = 0;
        while (*s != '\0') {
                hash = 31 * hash + *s ++;
        }
        return hash;
}

static int search(SET *sp, char *elt, bool *found) {			      // Searches if inputed element exists in array and provided index to add element to array if it is not found
        assert((sp != NULL) && (elt != NULL) && (found != NULL));             // O(n), with n being the total number of elements in the array if the array is full during probing
        int i;
	int hashIndex;
	int deletedIndex;
	int startIndex;
	deletedIndex = -1;
	startIndex = strhash(elt) % sp->len;
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
		if (strcmp(sp->data[hashIndex], elt) == 0) {
                        *found = true;
			return hashIndex;
                }
	}
	return -1;
}

void destroySet(SET *sp) {							// Deletes flags array and sp array, and free their allocated memory
	assert(sp != NULL);							// O(n), with n being the total number of elements in the array
	int i;
	for (i = 0; i <	sp->len; i++) {
		if (sp->flags[i] == 'F') {
			free(sp->data[i]);
		}
	}
	free(sp->flags);
	free(sp->data);
	free(sp);
}

int numElements(SET *sp) {							// Returns # of elements in array
	assert(sp != NULL);							// O(1), constant runtime completity
	return sp->count;
}

void addElement(SET *sp, char *elt) {						// Adds inputed element to array using search function to find index
        int index;								// O(n), since runtime complexity is constant, but calls search which has O(n) runtime
	bool found;
	char* copy;
	assert((sp != NULL) && (elt != NULL));

	index = search(sp, elt, &found);
	if (!found) {
		assert(sp->count < sp->len);
		copy = strdup(elt);
		assert(copy != NULL);
		sp->data[index] = copy;
                sp->count = sp->count + 1;               
		sp->flags[index] = 'F';
	}
}

void removeElement(SET *sp, char *elt) {					// Removes inputed element from array
	bool found;								// O(n), since runtime complexity is constant, but calls search which has O(n) runtime
	int index;
	assert((sp != NULL) && (elt != NULL));
	
        index = search(sp, elt, &found);
        if (found == true) {
                free(sp->data[index]);
		sp->flags[index] = 'D';
		sp->count = (sp->count - 1);
	}
}

char *findElement(SET *sp, char *elt) {						// Finds inputed element in array
	assert((sp != NULL) && (elt != NULL));					// O(n), since runtime complexity is constant, but calls search which has O(n) runtime
	bool found;
	int index;

	index = search(sp, elt, &found);
	if (found == true) {
		return sp->data[index];
	}
	return NULL;
}

char **getElements(SET *sp) {							// Returns new array of all elements in sp array
	assert(sp != NULL);							// O(n), with n being the total number of elements in the array
	char **tp;
	int i;
	int k;
	k = 0;
	tp = malloc(sizeof(char*) * sp->count);
	assert(tp != NULL);	
	
	for(i = 0; i < sp->len; i++) {
		if (sp->flags[i] == 'F') {
			tp[k] = sp->data[i];
			k = k + 1;
		}
	}
	
	return tp;
}


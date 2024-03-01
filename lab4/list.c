// list.c, Grant Johnson, COEN 12, 11/2/2023
// This program stores unique data from a file into a list using a linked lists. The provided functions are used by provided files to access/manipulate the stored  data type. The data type is determined by the user

#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct list {						      // Creates list def to establish list structure
	int count;
	struct node *head;
	int (*compare)();
}LIST;

typedef struct node{						      // Creates node def to provide pointers and data used within list
	void *data;
	struct node *next;
	struct node *prev;
}NODE;

LIST *createList(int (*compare)()) {				      // Creates a list with dummy node to be used by other functions
	struct list *lp;					      // O(1), since the runtime complexity is constant
	lp = malloc(sizeof(struct list));
	assert (lp != NULL);
	lp->count = 0;
	lp->compare = compare;
        lp->head = malloc(sizeof(struct node));
        assert(lp->head != NULL);
        lp->head->next = lp->head;
        lp->head->prev = lp->head;

	return lp;
}

void destroyList(LIST *lp) {					      // Destroys pointers within list and lp list
	assert(lp != NULL);					      // O(n), where n is total items in list
	struct node *pPrev;
	pPrev = malloc(sizeof(struct node));
	assert(pPrev != NULL);
	struct node *pDel;
	pDel = malloc(sizeof(struct node));
        assert(pDel != NULL);	
	pDel = lp->head->prev;

	while (pDel != lp->head) {
		pPrev = pDel->prev;
		free(pDel);
		pDel = pPrev;
	}
	free(pDel);
	free(lp);
}

int numItems(LIST *lp) {					      // Returns count of items in list
	assert(lp != NULL);					      // O(1), since item count is an existing constant
	return lp->count;
}

void addFirst(LIST *lp, void *item) {				      // Adds item to front of linked list
	assert((lp != NULL) && (item != NULL));	       		      // O(1), since runtime complexity is constant
	struct node *newEl;
	newEl = (malloc(sizeof(struct node)));
	assert(newEl != NULL);

	newEl->next = lp->head->next;
	newEl->prev = lp->head;
	newEl->data = item;
	lp->head->next->prev = newEl;
	lp->head->next = newEl;
	lp->count = lp->count + 1;
}

void addLast(LIST *lp, void *item) {			     	       //  Adds item to end of linked list
	assert((lp != NULL) && (item != NULL));			       //  O(1), since runtime complexity is constant
        struct node *newEl;
        newEl = (malloc(sizeof(struct node)));
        assert(newEl != NULL);

        newEl->next = lp->head;
        newEl->prev = lp->head->prev;
	newEl->data = item;
        lp->head->prev->next = newEl;
	lp->head->prev = newEl;
	lp->count = lp->count + 1;
}

void *removeFirst(LIST *lp) {					       // Removes first item in list
	assert((lp != NULL) && (lp->count > 0));		       // O(1), since runtime complexity is constant
	void* data;

	data = lp->head->next->data;
        lp->head->next->next->prev = lp->head;
	free(lp->head->next);
        lp->head->next = lp->head->next->next;
	lp->count = lp->count - 1;

	return data;
}
	
void *removeLast(LIST *lp) {						// Removes last item in list
  	assert((lp != NULL) && (lp->count > 0));			// O(1), since runtime complexity is constant	
	void* data;

	data = lp->head->prev->data;
        lp->head->prev->prev->next = lp->head;
	free(lp->head->prev);
        lp->head->prev = lp->head->prev->prev;
	lp->count = lp->count - 1;

	return data;
}

void *getFirst(LIST *lp) {					       // Returns first item in list
	assert(lp != NULL);					       // O(1), since runtime complexity is constant	
	return lp->head->next->data;
}

void *getLast(LIST *lp) {					       // Returns last item in list
        assert(lp != NULL);					       // O(1), since runtime complexity is constant	
        return lp->head->prev->data;
}

void removeItem(LIST *lp, void *item) {				       // Removes provided item from list if it exists
	assert((lp != NULL) && (item != NULL));			       // O(n), where n is total number of items in listO(n), where n is total number of items in list
        struct node *pPrev;
        pPrev = malloc(sizeof(struct node));
        assert(pPrev != NULL);
        pPrev = lp->head->prev;

        while (pPrev != lp->head) {
		if ((lp->compare)(pPrev->data, item) == 0) {
			pPrev->prev->next = pPrev->next;
			pPrev->next->prev = pPrev->prev;
			free(pPrev);
			lp->count = lp->count - 1;
			return;
		}
                pPrev = pPrev->prev;
        }
}

void *findItem(LIST *lp, void *item) {					// Finds provided item from list if it exists
	assert((lp != NULL) && (item != NULL));				// O(n), where n is total number of items in list
        struct node *pPrev;
        pPrev = malloc(sizeof(struct node));
        assert(pPrev != NULL);
        pPrev = lp->head->prev;

        while (pPrev != lp->head) {
                if ((lp->compare)(pPrev->data, item) == 0) {
                        return pPrev->data;
                }
                pPrev = pPrev->prev;
        }
	return NULL;
}

void *getItems(LIST *lp) {						// Provides array of all data from list
	assert(lp != NULL);						// O(n), where n is total number of items in list
	int i;
	i = 0;
	void **tp; 
	tp = malloc(sizeof(void*) * lp->count);
	assert(tp != NULL);	
	struct node *pNext;
        pNext = malloc(sizeof(struct node));
        assert(pNext != NULL);
        pNext = lp->head->next;

        while (pNext != lp->head) {
                tp[i] = pNext->data;
		i = i + 1;
        	pNext = pNext->next;
	}
	return tp;
}


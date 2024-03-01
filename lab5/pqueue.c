//Grant Johnson, COEN12, 11/17/2023
//Creates a priority queue to remove smallest values (highest priorities first) from an array
#include "pqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
# define addParent(x) (((x) - 1) / 2)

typedef struct pqueue {
	int count;
	int length;
	void **data;
	int (*compare)();
} PQ;

PQ *createQueue(int (*compare)()) {           //Creates queue and sets initial values, O(1)
	assert (compare != NULL);
	PQ *pq;
	pq = malloc(sizeof(pq));
	assert(pq != NULL);
	pq->compare = compare;
	pq->length = 10;
	pq->count = 0;
	pq->data = malloc(sizeof(void*) * pq->length);
	assert(pq->data != NULL);

	return pq;
}

void destroyQueue(PQ *pq) {                   //Free allocated memory, O(1)
	assert (pq != NULL);

	free(pq->data);
	free(pq);
}

int numEntries(PQ *pq) {                      //Returns number of elements, O(1)
  assert(pq != NULL);
  return pq->count;
}

void addEntry(PQ *pq, void *entry) {           //Adds element to queue and requeues it up if needed, O(log(n)) since runtime depends of depth of tree
	assert((pq != NULL) && (entry != NULL));
	int current;
	
	if (pq->count == pq->length) {
		pq->length = pq->length * 2;
		pq->data = realloc(pq->data, sizeof(void*) * pq->length);
		assert(pq->data != NULL);
	}
	
      
	current = pq->count;
	while ((current > 0) && ((*pq->compare)(pq->data[addParent(current)], entry) > 0)) {
	  pq->data[current] = pq->data[addParent(current)];
	  current = addParent(current);
	}
	pq->data[current] = entry;
	pq->count = pq->count + 1;
}

void *removeEntry(PQ *pq) {                  //Removes element to queue and requeues it updown if needed, O(log(n)) since runtime depends of depth of tree
	assert (pq != NULL);
	void *removedElt;
	void *tempElt;
	int smallerIndex;
	int current;
	int leftChild;
	int rightChild;

	removedElt = pq->data[0];
	pq->count = pq->count - 1;
	pq->data[0] = pq->data[pq->count];
	
	current = 0;
	leftChild = 1;
	rightChild = 2;
	while (leftChild < pq->count) {
	  if (rightChild < pq->count) {
    		if ((*pq->compare)(pq->data[leftChild], pq->data[rightChild]) > 0) {
		     smallerIndex = rightChild;
	    	}
	    	else {
		     smallerIndex = leftChild;
	        }
	  }
	  else {
	    smallerIndex = leftChild;
	  }
	  	if ((*pq->compare)(pq->data[current], pq->data[smallerIndex]) > 0) {
	    		tempElt = pq->data[current];
	    		pq->data[current] = pq->data[smallerIndex];
	    		pq->data[smallerIndex] = tempElt;
	  	}
	  	else {
	    		break;
	  	}
		current = smallerIndex;
		leftChild = (current * 2) + 1;
		rightChild = (current * 2) + 2;
	}
	return removedElt;
}

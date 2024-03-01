/*Grant Johnson, COEN12, 11/25/23
The program uses a doubly-linked, circular list of arrays to sort elements of a user-provided data type. The array sizes increase as they are filled.
*/
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <assert.h>
#define INITIAL_CAPACITY 500

// Structure definition for the circular queue
typedef struct {
    void **array;
    int capacity;
    int front;
    int rear;
    int size;
} CircularQueue;

// Structure definition for linked list nodes
typedef struct Node {
    CircularQueue *queue;
    struct Node *next;
    struct Node *prev;
} Node;

// Structure definition for linked list
struct list {
    Node *head;
    Node *tail;
    int totalItems;
int numNodes;
} ;

/*
Function: createList
Creates a new list
O(1)
*/

LIST *createList() {
    LIST *list = (LIST *)malloc(sizeof(LIST));
    assert(list != NULL && "Memory allocation failed");
    list->head = list->tail = NULL;
    list->totalItems = 0;
    list->numNodes = 0;

    
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->queue = (CircularQueue *)malloc(sizeof(CircularQueue));
    newNode->queue->array = (void **)malloc(500 * sizeof(void *));
    newNode->queue->capacity = 500;
    newNode->queue->front = newNode->queue->rear = newNode->queue->size = 0;
    newNode->next = newNode->prev = NULL;

    list->head = list->tail = newNode;
    list->numNodes++;

    return list;
}

/*
Function: destroyList
Deallocates all memory allocated by program
O(n)
*/

void destroyList(LIST *lp) {
    if (lp == NULL) return;

    Node *current = lp->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp->queue->array);
        free(temp->queue);
        free(temp);
    }

    free(lp);
}

/*
Function: numItems
Returns number of items in the list.
O(1)
*/

int numItems(LIST *lp) {
    return (lp != NULL) ? lp->totalItems : 0;
}

/*
Function: addLast
Resizes list if needed and adds provided item to the last location of list
O(1)
*/

void addLast(LIST *lp, void *item) {
    assert(lp != NULL && "Invalid list pointer");

    if (lp->head == NULL || lp->tail == NULL) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->queue = (CircularQueue *)malloc(sizeof(CircularQueue));
        newNode->queue->array = (void **)malloc(INITIAL_CAPACITY * sizeof(void *));
        newNode->queue->capacity = INITIAL_CAPACITY;
        newNode->queue->front = newNode->queue->rear = newNode->queue->size = 0;
        newNode->next = newNode->prev = NULL;

        lp->head = lp->tail = newNode;
        lp->numNodes++;
    }

    if (lp->tail->queue->size == lp->tail->queue->capacity) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->queue = (CircularQueue *)malloc(sizeof(CircularQueue));
        newNode->queue->array = (void **)malloc(lp->head->queue->capacity * lp->numNodes * sizeof(void *));
        newNode->queue->capacity = lp->head->queue->capacity * lp->numNodes;
        newNode->queue->front = 0;
        newNode->queue->rear = 0;
        newNode->queue->size = 0;
        newNode->next = NULL;
        newNode->prev = lp->tail;

        lp->tail->next = newNode;
        lp->tail = newNode;
        lp->numNodes++;
    }

    lp->tail->queue->array[lp->tail->queue->rear] = item;
    lp->tail->queue->rear = (lp->tail->queue->rear + 1) % lp->tail->queue->capacity;
    lp->tail->queue->size++;
    lp->totalItems++;
}

/*
Function: removeFirst
Removes first item in list, adjusts circular queue and node structures, and returns the removed item
O(1)
*/

void *removeFirst(LIST *lp) {
    assert(lp != NULL && "Invalid list pointer");
    if (lp == NULL || lp->head == NULL) return NULL;

    if (lp->head->queue->size == 0) {
        if (lp->totalItems == 0) {
            return NULL;
        }

        Node *temp = lp->head;
        lp->head = lp->head->next;
        if (lp->head != NULL) {
            lp->head->prev = NULL;
        }
        free(temp->queue->array);
        free(temp->queue);
        free(temp);
        lp->numNodes--;
        lp->totalItems--;

        return NULL;
    }

    void *item = lp->head->queue->array[lp->head->queue->front];
    lp->head->queue->front = (lp->head->queue->front + 1) % lp->head->queue->capacity;
    lp->head->queue->size--;
    lp->totalItems--;

    if (lp->head->queue->size == 0) {
        Node *temp = lp->head;
        lp->head = lp->head->next;
        if (lp->head != NULL) {
            lp->head->prev = NULL;
        }
        free(temp->queue->array);
        free(temp->queue);
        free(temp);
        lp->numNodes--;
    }

    return item;
}

/*

Function: removeLast
Removes last item in list, adjusts circular queue and node structures, and returns the removed item
O(1)
*/

void *removeLast(LIST *lp) {
    assert(lp != NULL && "Invalid list pointer");
    if (lp == NULL || lp->tail == NULL) return NULL;

    if (lp->tail->queue->size == 0) {
        if (lp->totalItems == 0) {
            free(lp->tail->queue->array);
            free(lp->tail->queue);
            free(lp->tail);
            lp->head = lp->tail = NULL;
            return NULL;
        }

        Node *temp = lp->tail;
        lp->tail = lp->tail->prev;
        if (lp->tail != NULL) {
            lp->tail->next = NULL;
        }
        free(temp->queue->array);
        free(temp->queue);
        free(temp);
        lp->numNodes--;

        lp->totalItems--;

        return NULL;
    }

    void *item = lp->tail->queue->array[lp->tail->queue->rear];
    lp->tail->queue->rear = (lp->tail->queue->rear - 1 + lp->tail->queue->capacity) % lp->tail->queue->capacity;
    lp->tail->queue->size--;

    if (lp->tail->queue->size == 0) {
        Node *temp = lp->tail;
        lp->tail = lp->tail->prev;
        if (lp->tail != NULL) {
            lp->tail->next = NULL;
        }
        free(temp->queue->array);
        free(temp->queue);
        free(temp);
        lp->numNodes--;
    }

    lp->totalItems--;

    return item;
}

/*
Function: addFirst
Resizes circular queue if needed and adds an item to the first location list
O(1)
*/

void addFirst(LIST *lp, void *item) {
    assert(lp != NULL && "Invalid list pointer");
    if (lp == NULL) return;

    if (lp->head->queue->size == lp->head->queue->capacity) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->queue = (CircularQueue *)malloc(sizeof(CircularQueue));
        newNode->queue->array = (void **)malloc((lp->head->queue->capacity*lp->numNodes) * sizeof(void *));  // Initial array capacity of 100
        newNode->queue->capacity = (lp->head->queue->capacity*lp->numNodes);
        newNode->queue->front = 0;
        newNode->queue->rear = 0;
        newNode->queue->size = 0;
        newNode->next = lp->head;
        newNode->prev = NULL;

        lp->head->prev = newNode;
        lp->head = newNode;
        lp->numNodes++;
    }

    lp->head->queue->front = (lp->head->queue->front - 1 + lp->head->queue->capacity) % lp->head->queue->capacity;

    lp->head->queue->array[lp->head->queue->front] = item;
    lp->head->queue->size++;
    lp->totalItems++;
}

/*
Function: getItem
Returns item at provided index in list.
O(n)
*/

void *getItem(LIST *lp, int index) {
    if (lp == NULL || index < 0 || index >= lp->totalItems) return NULL;

    Node *current = lp->head;
    int adjustedIndex = index;
    while (current != NULL) {
        if (adjustedIndex < current->queue->size) {
            
            return current->queue->array[(current->queue->front + adjustedIndex) % current->queue->capacity];
        } else {
            
            adjustedIndex -= current->queue->size;
            current = current->next;
        }
    }

    return NULL; 
}

/*
Function: setItem
Sets the item at provided index in the list to provided value.
O(n)
*/

void setItem(LIST *lp, int index, void *item) {
    assert(lp != NULL && "Invalid list pointer");
    assert(index >= 0 && index < lp->totalItems && "Index out of range");

    Node *current = lp->head;
    int adjustedIndex = index;
    while (current != NULL) {
        if (adjustedIndex < current->queue->size) {
          
            int arrayIndex = (current->queue->front + adjustedIndex) % current->queue->capacity;
            current->queue->array[arrayIndex] = item;
            return;
        } else {
           
            adjustedIndex -= current->queue->size;
            current = current->next;
        }
    }

    assert(0 && "Failed to set item");
}

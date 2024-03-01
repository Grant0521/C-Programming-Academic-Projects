//Grant Johnson, COEN12, 11/17/2023
//Uses the Huffman coding algorithm to compress characters in a file based on frequency
#include "pqueue.h"
#include "pack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

typedef struct node NODE;

// Creates a node
// O(1), since there is a constant runtime complexity
static NODE *mknode(int data, NODE *left_node, NODE *right_node) {
    NODE *p = malloc(sizeof(NODE));
    p->count = data;
    p->parent = NULL;
    if (left_node != NULL)
        left_node->parent = p;
    if (right_node != NULL)
        right_node->parent = p;
    return p;
}

// Compare two node pointers
// O(1), since there is a constant runtime complexity
static int compare(NODE *t1, NODE *t2) {
    return (t1->count < t2->count) ? -1 : (t1->count > t2->count);
}

// Finds depth by looping down the tree
// O(n), since there is a loop based on depth
static int depth(NODE *p) {
    int depth = 0;
    while (p->parent != NULL) {
        p = p->parent;
        depth++;
    }
    return depth;
}

int main(int argc, char *argv[]) {
    int i, c;                             // Initializes variables and opens text file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening input file");
        return 1;
    }
    int counts[257] = {0};
    NODE *nodes[257] = {0};
    for (i = 0; i < 257; i++) {
        nodes[i] = NULL;
    }
    while (1) {
        c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
        counts[c]++;
    }
    PQ *pq = createQueue(compare); 
    int j;
    for (j = 0; j < 257; j++) {
        if (counts[j] > 0) {              
            NODE *n = mknode(counts[j], NULL, NULL);
            addEntry(pq, n);                     // O(log(n)), where n is the number of entries in the priority queue
            nodes[j] = n;
        }
    }
    NODE *eof = mknode(0, NULL, NULL);
    addEntry(pq, eof);                           // O(log(n)), where n is the number of entries in the priority queue
    nodes[256] = eof;
    while (numEntries(pq) > 1) {
        NODE *f = removeEntry(pq);               // O(log(n)), where n is the number of entries in the priority queue
        NODE *s = removeEntry(pq);               // O(log(n)), where n is the number of entries in the priority queue
        NODE *plus = mknode(f->count + s->count, f, s);
        addEntry(pq, plus);                      // O(log(n)), where n is the number of entries in the priority queue
    }
    int k;
    for (k = 0; k < 257; k++) {
        if (nodes[k] != NULL) {
            if (isprint(k)) {
                printf("'%c':%d x %d bits =%d bits\n", k, counts[k], depth(nodes[k]), counts[k] * depth(nodes[k]));
            } else {
                printf("%03o : %d x %d bits =%d bits\n", k, counts[k], depth(nodes[k]), counts[k] * depth(nodes[k]));
            }
        }
    }
    pack(argv[1], argv[2], nodes);               // Big O implication from pack depends on usage of function
    return 0;
}

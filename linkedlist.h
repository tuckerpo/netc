#pragma once
#ifndef __LLIST_H
#define __LLIST_H

#include <stdlib.h>

typedef struct Node {
	void* data;
	struct Node* next;
} Node;

typedef struct {
	Node* head;
	int size;
} LinkedList;

/* Return a ptr to a newly alloc'd list */
LinkedList *llist_create();

/* Free heap allocated memory */
void llist_free (LinkedList*);

/* Return number of nodes in the list */
int llist_size(LinkedList*);

/* Head insertion */
void *llist_insert(LinkedList*, void*);

/* Tail insertion */
void *llist_append(LinkedList*, void*);

/* Find an element */
void *llist_find(LinkedList*, void*, int (*cmpfunc)(void *, void*));

/* Walk the list and apply func to each node's data */
void llist_for_each(LinkedList*, void (*func)(void *, void*), void *);

#endif // __LLIST_H
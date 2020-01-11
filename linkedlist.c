#include "linkedlist.h"

LinkedList* llist_create() {
	return (LinkedList*)malloc(sizeof(LinkedList));
}

void llist_free(LinkedList *ll) {
	Node *n = ll->head;
	Node *next;
	while (n != NULL) {
		next = n->next;
		free(n);
		n = next;
	}
	free(ll);
}

void* llist_insert(LinkedList* ll, void* data) {
	Node* n = malloc(sizeof(n));
	if (n) {
		n->data = data;
		n->next = ll->head;
		ll->head = n;
		ll->size++;
		return data;
	}
	return NULL;
}

void *llist_append(LinkedList* ll, void* data) {
	Node* tail = ll->head;

	if (tail == NULL) {
		/* List head points to nothing, insert at front. */
		return llist_insert(ll, data);
	}

	Node* n = malloc(sizeof(Node));

	if (n) {
		while (tail->next != NULL) {
			tail = tail->next;
		}
		n->data = data;
		tail->next = n;
		ll->size++;
		return data;
	}
	return NULL;
}

void *llist_find(LinkedList* ll, void *data, int (*cmpfn)(void *, void*)) {

	Node *n = ll->head;

	if (n) {
		while (n != NULL) {
			if (cmpfn(data, n->data) == 0) {
				break;
			}
			n = n->next;
		}
		if (n) {
			/* If n is not NULL here, cmpfn returned success on some node. Return data at that node. */
			return n->data;
		}
	}

	/* List was either empty, of the cmpfn did not match. */
	return NULL;
}

void llist_for_each(LinkedList* ll, void (*func)(void *, void*), void* arg) {
	Node *n = ll->head;
	Node *next;
	if (n) {
		while (n != NULL) {
			next = n->next;
			func(n->data, arg);
			n = next;
		}
	}
}

int llist_size(LinkedList* ll) {
	return ll->size;
}

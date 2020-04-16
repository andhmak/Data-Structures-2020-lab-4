///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω ταξινομημένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTList.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί List


int* create_char(char a) {
    int* p = malloc(sizeof(int));
	*p = a;
	return p;
}

// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	List list;		// η λίστα στην οποία αποθηκεύουμε τα στοιχεία
	CompareFunc compare;
};

// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

void pqueue_insert(PriorityQueue pqueue, Pointer value);

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	// Για την κλήση της συνάρτησης destroy_value, την περνάμε απλά στη list_create
	// οπότε θα καλείται αυτόματα όταν το στοιχείο αφαιρεθεί από τη λίστα.
	// ... = list_create(destroy_value);
	pqueue->list = list_create(destroy_value);
	// Ένας dummy πρώτος κόμβος ώστε αν πρέπει να αλλάξει ο πραγματικός πρώτος κόμβος
	// να γίνεται εύκολα (δεν έχω πρόσβαση στον dummy της υλοποίησης της λίστας)
	list_insert_next(pqueue->list, LIST_BOF, create_char(0));
	pqueue->compare = compare;
	if (values != NULL) {
		for (uint i = 0 ; i < vector_size(values) ; i++) {
			pqueue_insert(pqueue, vector_get_at(values, i));
		}
	}
	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	// Αφαίρεση 1 λόγω του dummy
	return list_size(pqueue->list) - 1;
}

Pointer pqueue_max(PriorityQueue pqueue) {
	ListNode max = list_next(pqueue->list, list_first(pqueue->list));
	return list_node_value(pqueue->list, max);
}

void pqueue_insert(PriorityQueue pqueue, Pointer value) {
	// Αν υπάρχει μόνο ο dummy, απλά προστίθεται το στοιχείο
	if (list_size(pqueue->list) == 1) {
		list_insert_next(pqueue->list, list_first(pqueue->list), value);
		return;
	}
	// Αλλιώς προστίθεται πριν το πρώτο (μεγαλύτερο) μικρότερό του
	for (ListNode node = list_first(pqueue->list) ; list_next(pqueue->list, node) != LIST_EOF ; node = list_next(pqueue->list, node)) {
		if (pqueue->compare(list_node_value(pqueue->list, list_next(pqueue->list, node)), value) < 0) {
			list_insert_next(pqueue->list, node, value);
			return;
		}
	}
	list_insert_next(pqueue->list, list_last(pqueue->list), value);
}

void pqueue_remove_max(PriorityQueue pqueue) {
	list_remove_next(pqueue->list, list_first(pqueue->list));
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	// Απλά καλούμε την αντίστοιχη συνάρτηση της λίστας
	return list_set_destroy_value(pqueue->list, destroy_value);
}

void pqueue_destroy(PriorityQueue pqueue) {
	list_destroy(pqueue->list);
	free(pqueue);
}

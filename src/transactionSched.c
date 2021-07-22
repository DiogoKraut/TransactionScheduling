/* Diogo Paris Kraut - GRR 20166365 */
/*! \file transactionSched.c
 * \brief Functions for working with a transaction list
 */
#include <string.h>
#include "transactionSched.h"

/*! Allocates and initiates a transaction list */
transaction_list_t *initTransactionList() {
	transaction_list_t *t = malloc(sizeof(transaction_list_t));
	t->head = NULL;
	t->size = 0;

	return t;
}

/*! Allocates and initiates a transaction *
 * \param time Timestamp in log
 * \param id Transaction id
 * \param op Operation
 * \param atr Attribute being operated on
 */
transaction_t *initTransaction(int time, int id, char op, char *atr) {
	transaction_t *t = malloc(sizeof(transaction_t));
	t->next = NULL;

	t->time = time;
	t->id = id;
	switch(op) {
		case 'r':
		case 'R':
			t->op = READ;
			break;
		case 'w':
		case 'W':
			t->op = WRITE;
			break;
		case 'c':
		case 'C':
			t->op = COMMIT;
			break;
	}
	t->commit = 0;
	strcpy(t->atr, atr);

	return t;
}

/*! Sets committed status on transactions with id 'id'
 * \param l Pointer to transaction list
 * \param id id of commit
 */
void commitTransaction(transaction_list_t *l, int id) {
	transaction_t *aux = l->head;

	while(aux != NULL) {
		if(aux->id == id)
			aux->commit = 1;
		aux = aux->next;
	}
}

/*! Searches list 'l' for transaction with id 'id'
 * \param l Pointer to transaction list
 * \param id Id being searched for
 * \return 1 If transaction was found
 * \return 0 Otherwise
 */
int transactionIdSearch(transaction_list_t *l, int id) {
	transaction_t *aux = l->head;
	while(aux != NULL) {
		if(aux->id == id)
			return 1;
		aux = aux->next;
	}
	return 0;
}

/*! Frees the transactions in a list, but keeps the reference to the list itself
 * \param l Pointer to transaction list
 */
void freeTransactionList(transaction_list_t *l) {
	transaction_t *aux = l->head;
	transaction_t *aux2 = l->head;
	while(aux != NULL) {
		aux2 = aux;
		aux = aux->next;
		free(aux2);
	}
	l->head = NULL;
	l->size = 0;
}
/*! Removes transaction with 'id' from list 'l' 
 * \param l Pointer to transaction list
 * \param id id to be removed 
 */
void removeTransaction(transaction_list_t *l, int id) {
	if(l == NULL) {
		fprintf(stderr, "appendTransaction: argument cant be NULL\n");
		exit(1);
	}

	transaction_t *aux = l->head;
	transaction_t *auxFree;
	if(aux->id == id) {
		l->head = aux->next;
		l->size--;
		free(aux);
		return;
	}

	while(aux->next != NULL) {
		if(aux->next->id == id) {
			auxFree	= aux->next;
			aux->next = aux->next->next;
			free(auxFree);
			return;
		}
		aux = aux->next;
	}

}

/*! Insert at the end of the list 
 * \param l Pointer to transaction list
 * \param t Pointer to transaction to be inserted
 */
void appendTransaction(transaction_list_t *l, transaction_t *t) {
	if(l == NULL || t == NULL) {
		fprintf(stderr, "appendTransaction: argument cant be NULL\n");
		exit(1);
	}

	transaction_t *aux = l->head;

	if(aux == NULL) {
		l->head = t;
	} else {
		while(aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = t;
		t->next = NULL;
	}
	l->size++;
}

/*! Returns a clone of the 'src' transaction
 * \param src Pointer to transaction to be cloned
 */
transaction_t *cloneTransaction(transaction_t *src) {
	transaction_t *aux = malloc(sizeof(transaction_t));

	aux->id = src->id;
	aux->op = src->op;
	aux->time = src->time;
	aux->commit = 0;
	strcpy(aux->atr, src->atr);
	aux->next = NULL;

	return aux;
}
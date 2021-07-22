/* Diogo Paris Kraut - GRR 20166365 */
/*! \file transactionSched.h
 * \brief Structures and prototypes for working with transaction litsts
 */
#include <stdio.h>
#include <stdlib.h>

/*! MAX_INPUT Maximum input lenght */
#define MAX_INPUT 50
/*! READ Encoding for read operation */
#define READ  0  
/*! Encoding for write operation */
#define WRITE 1
/*! Encoding for commit operation */  
#define COMMIT 2 

/*! Holds the info of a transaction */
typedef struct transaction_t {
	struct transaction_t *next; /*!< Points to the next element of the llist */
	int id; 			 /*!< Transaction id */
	int op; 			 /*!< Operation */
	char atr[MAX_INPUT]; /*!< Attribute being operated on */
	int commit;		     /*!< Has a commit been received */
	int time;			 /*!< Timestamp in log */
} transaction_t;

/*! Transaction list structure */
typedef struct transaction_list_t {
	int size;			 /*!< Size of list */
	transaction_t *head; /*!< First element of list */
}transaction_list_t;

void freeTransactionList(transaction_list_t *l);
transaction_list_t *initTransactionList();
transaction_t *initTransaction(int time, int id, char op, char *atr);
void commitTransaction(transaction_list_t *l, int id);
int transactionIdSearch(transaction_list_t *l, int id);
void removeTransaction(transaction_list_t *t, int id);
void appendTransaction(transaction_list_t *l, transaction_t *t);
transaction_t *cloneTransaction(transaction_t *src);
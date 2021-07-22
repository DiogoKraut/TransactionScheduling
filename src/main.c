/* Diogo Paris Kraut - GRR 20166365 */
/*!\file main.c
 * \brief Driver code and problem-specific functions 
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "transactionSched.h"
#include "graph.h"

/*! Generates the conflict graph from the transaction list 'l'.
 * First we make a vertex for every transaction in the list.
 * Then we grab every permutation of 2 different transactions.
 * If either one is WRITE and the other READ or if they are both WRITE,
 * then we create an edge between them.
 * \param l Pointer to transaction list
 * \return Returns a pointer to the generated graph
 */
graph_t *graphGen(transaction_list_t *l) {
	graph_t *g = initGraph();
	transaction_t *t_aux1, *t_aux2, *t_auxA, *t_auxB;
	vertex_t *v_auxA, *v_auxB;

	/* add vertex for every transaction */
	t_aux1 = l->head;
	while(t_aux1 != NULL) {
		if(vertexSearch(g, t_aux1->id) == NULL)
			appendList(g->vertex, initNode(initVertex(t_aux1->id)));
		t_aux1 = t_aux1->next;
	}

	/* grab pairs of transactions */
	t_aux1 = l->head;
	while(t_aux1 != NULL) {
		t_auxA = t_aux1;

		t_aux2 = t_aux1->next;
		while(t_aux2 != NULL) {
			t_auxB = t_aux2;
			/* If they operate on same attribute, but are different transactions */
			if(strcmp(t_auxA->atr, t_auxB->atr) == 0 && t_auxA->id != t_auxB->id) {
				/* if either A or B is WRITE and the other READ or if they are both WRITE */
				if((t_auxA->op == WRITE && t_auxB->op == READ) 
				|| (t_auxA->op == READ && t_auxB->op == WRITE)
				|| (t_auxA->op == WRITE && t_auxB->op == WRITE)) {
					/* Create edge from A to B */
					v_auxA = vertexSearch(g, t_auxA->id);
					v_auxB = vertexSearch(g, t_auxB->id);

					if(!edgeSearch(g, initEdge(v_auxA, v_auxB))) {
						appendList(g->edges, initNode(initEdge(v_auxA, v_auxB)));
					}
				}
			} 
			t_aux2 = t_aux2->next;
		}
		t_aux1 = t_aux1->next;
	}
	return g;
}
/*! \brief Driver code
 * 
 * First we read the input and parse it into a list of transactions. 
 * Then we split that list into two, one that holds 1 member per transaction id (active_list)
 * and one that keeps track of transactions we've already passed through (visited_list). 
 * 
 * When we find a commit operation we reduce the size of active_list. When that gets to zero
 * we know all competing transactions have been committed and we can determine if its 
 * serializable by conflict or equivalent by vision, print the result, and move on to the
 * next set of transactions
 * 
 */
int main(int argc, char const *argv[]) {
	/* INITIALIZATIONS */
	graph_t *graph;
	transaction_list_t *t_list = initTransactionList();
	transaction_t *aux;
	int time, id, count = 1;
	int print_count = 0;
	char op;
	char atr[MAX_INPUT];

	char *line = NULL;
	size_t n = 0;

	/* INPUT PARSING */
	/* Read line from input*/
	while(getline(&line, &n, stdin) != -1) {
		sscanf(line, "%d %d %c %s", &time, &id, &op, atr); // parse line 
		aux = initTransaction(time, id, op, atr);		   // add to transaction struct

		if(aux->op == COMMIT)
			commitTransaction(t_list, id);//inactive
		appendTransaction(t_list, aux); // add to lsit of transactions
	}

	// printlist(t_list);

	/* GOING THROUGH TRANSACTIONS */
	transaction_list_t *visited_list, *active_list;
	visited_list = initTransactionList();
	active_list  = initTransactionList();

	aux = t_list->head;
	transaction_t *active_aux;
	while(aux != NULL) {
		if(aux->op != COMMIT) {
			/* Add to auxiliar lists */
			if(!transactionIdSearch(active_list, aux->id)) {
				appendTransaction(active_list, cloneTransaction(aux));
			}
			/* append to visited list */
			appendTransaction(visited_list, cloneTransaction(aux));
		} else {
			active_list->size--;

			if(active_list->size == 0) {
				char *s1 = malloc(128);
				char *s2 = malloc(128);
				char *s3 = malloc(128);
				active_aux = active_list->head;
				/* Format transaction id list */
				while(active_aux != NULL) {
					if(print_count > 0)
						sprintf(s1, ",%d", active_aux->id);
					else
						sprintf(s1, "%d", active_aux->id);
					strcat(s2, s1);
					active_aux = active_aux->next;
					print_count++;
				}
				sprintf(s3, "%d %s", count, s2);

				/* Generate graph */
				graph = graphGen(visited_list);

				/* Determine if its serilizable */
				if(hasCycle(graph)) 
					strcat(s3, " NS");
				else
					strcat(s3, " SS");

				// if(visionEquivalent(visited_list)) 
				// 	sprintf(s2, "%s NV", s2);
				// else
				// 	sprintf(s2, "%s SV", s2);
				
				printf("%s\n", s3);
				freeTransactionList(active_list);
				freeTransactionList(visited_list);
				count++;
				print_count = 0;
			}


		}
		aux = aux->next;
	}
	return 0;
}
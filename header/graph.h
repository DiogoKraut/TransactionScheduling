/* Diogo Paris Kraut - GRR 20166365 */
/*! \file graph.h
 * \brief Structures and prototypes for working with graphs
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*! Structure to hold generic data */
typedef struct node_t {
	struct node_t *next;
	void *data;
} node_t;

/*! List structure */
typedef struct list_t {
	int size;	  /*!< Size of list */
	node_t *head; /*!< First elem of list */
} list_t;

/*! Structure to represent a vertex of a graph */
typedef struct vertex_t {
	int id;			  /*!< id */
	int label;		  /*!< Labeling for BFS through graph */
	int visited; 	  /*!< Visited flag */
	list_t *edge_in;  /*!< Edges current vertex is destination of*/
	list_t *edge_out; /*!< Edges current vertex is the source of */
} vertex_t;

/*! Structure that represents an edge of a graph */
typedef struct edge {
	vertex_t *src; /*!< Source vertex */
	vertex_t *dest;/*!< Destination vertex */
} edge_t;

/*! Structure for the graph proper */
typedef struct graph_t {
	list_t *vertex;/*!< List of vertexes */
	list_t *edges; /*!< List of edges */
} graph_t;

list_t *initList();
node_t *initNode(void *data);
vertex_t *initVertex(int id);
edge_t *initEdge(vertex_t *src, vertex_t *dest);
graph_t *initGraph();
void appendList(list_t *l, node_t *n);
vertex_t *vertexSearch(graph_t *g, int id);
int edgeSearch(graph_t *g, edge_t *e);
void push(list_t *s, node_t *n);
node_t *pop(list_t *s);
int hasCycle(graph_t *g);
/* Diogo Paris Kraut - GRR 20166365 */
/*!\file graph.c *
 * \brief Functions for manipulating a graph (and the list/stack)
 */
#include "graph.h"


/*! Allocates and initiates a transaction list */
list_t *initList() {
    list_t *l = malloc(sizeof(list_t));

    l->head = NULL;
    l->size = 0;
    return l;
}

/*! Allocates and initiates a data node *
 *\param data Pointer to structure/var that holds data to be added to node
 */
node_t *initNode(void *data) {
    node_t *n = malloc(sizeof(node_t));

    n->data = data;
    n->next = NULL;

    return n;
}

/*! Allocates and initiates a vertex *
 *\param id Vertex 'id'
 */
vertex_t *initVertex(int id) {
    vertex_t *v = malloc(sizeof(vertex_t));

    v->edge_in  = initList();
    v->edge_out = initList();

    v->id = id;
    v->label = 0;
    v->visited = 0;

    return v;
}

/*! Allocates and initiates an edge     
 *\param src Source vertex of edge      
 *\param dest Destination vertex of edge 
 */
edge_t *initEdge(vertex_t *src, vertex_t *dest) {
    edge_t *e = malloc(sizeof(edge_t));

    e->src = src;
    e->dest = dest;
    appendList(e->src->edge_out, initNode(e));
    appendList(e->dest->edge_in, initNode(e));

    return e;
}

/*! Allocates and initiates a graph */
graph_t *initGraph() {
    graph_t *g = malloc(sizeof(graph_t));
    g->vertex = initList();
    g->edges  = initList();
    return g;
}

/*! Apeends node 'n' to list 'l'
 *\param l Pointer to a list
 *\param n Pointer to a node 
 */
void appendList(list_t *l, node_t *n) {
    node_t *aux = l->head;

    if(aux == NULL) {
        l->head = n;
    } else {
        while(aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = n;
        n->next = NULL;
    }
    l->size++;
}

/*! Searches graph 'g' for vertex with id 'id'
 *\param g Pointer to graph                    
 *\param id Id of vertex to look for           
 */
vertex_t *vertexSearch(graph_t *g, int id) {
    node_t *n_aux;
    vertex_t *v_aux;

    n_aux = g->vertex->head;
    while(n_aux != NULL) {
        v_aux = (vertex_t *)n_aux->data;
        if(v_aux->id == id)
            return v_aux;
        n_aux = n_aux->next;
    }

    return NULL;
}

/*! Searches graph 'g' for edge 'e'         
 *\param g Pointer to graph                 
 *\param e Pointer to edge to be looked for 
 */
int edgeSearch(graph_t *g, edge_t *e) {
    node_t *n_aux = g->edges->head;
    edge_t *e_aux;

    while(n_aux != NULL) {
        e_aux = (edge_t *)n_aux->data;
        if(e_aux->src == e->src && e_aux->dest == e->dest)
            return 1;
        n_aux = n_aux->next;
    }

    return 0;
}

/*! Stack operation push    
 *\param s Pointer to stack 
 *\param n Pointer to node  
 */
void push(list_t *s, node_t *n) {
    node_t *n_aux = s->head;
    if(n_aux != NULL) {
        n->next = n_aux;
        s->head = n;
    } else {
        s->head = n;
    }
    s->size++;
}

/*! Stack operation pop     
 *\param s Pointer to stack 
 */
node_t *pop(list_t *s) {
    node_t *n = s->head;
    s->head = s->head->next;
    s->size --;
    return n;
}

/*!
 * Function looks for cycles in graph 'g'                               
 * First we look for a starting vertex. It must have outgoing edges
 * If we cant find a vertex, the graph is disconnected and non-cyclic 
 *                                                                         
 * Apply Breadth First Search from the starting vertex, marking the vertexes
 * that were already visited. If we pass by an already marked vertex, the   
 * graph has a cycle                                                        
 *                                                                          
 *\param g Pointer to graph                                                 
 *\return 1 If 'g' is cyclic                                                
 *\return 0 If 'g' not non-cyclic                                           
 */
int hasCycle(graph_t *g) {
    vertex_t *v_aux;
    int start_found = 0;
    edge_t *e_aux;

    node_t *n_aux2;
    list_t *stack = initList();

    int count = 0;

    /* Find starting vertex */
    node_t *n_aux1 = g->vertex->head;
    while(n_aux1 != NULL && start_found == 0) {
        v_aux = (vertex_t *)n_aux1->data;
        if(v_aux->edge_out->size > 0) { //if has outgoing edges
            push(stack, initNode(v_aux));
            start_found = 1;//good starting node, stop searching
        }
        n_aux1 = n_aux1->next;
    }

    if(v_aux == NULL) {/* no vertex has outgoing edge */
        return 0;
    }

    count++;

    /* breadth first search */
    while(stack->size > 0) {
        n_aux2 = pop(stack);
        v_aux = (vertex_t *)n_aux2->data;
        if(!v_aux->visited) {
            v_aux->visited = 1;

            n_aux1 = v_aux->edge_out->head;
            /* push outgoing edges onto stack */
            while(n_aux1 != NULL) {
                e_aux = (edge_t *)n_aux1->data;
                if(e_aux->dest != NULL) {
                    if(e_aux->dest->label == 0) {
                        e_aux->dest->label = 1;
                        push(stack, initNode(e_aux->dest));
                        count++;
                    }
                }
                n_aux1 = n_aux1->next;
            }
            if(g->vertex->size > count) { //if we havent visited every vertex
                n_aux1 = g->vertex->head;
                while(n_aux1 != NULL) {
                    v_aux = (vertex_t *)n_aux1->data;
                    if(v_aux->visited == 0) {
                        push(stack, initNode(v_aux));
                        break;
                    }
                    n_aux1 = n_aux1->next;
                }
            }
        } else { //has cycle
            return 1;
        }
    }
    return 0;
}
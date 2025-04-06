#pragma once

typedef unsigned uint;

typedef enum Type { Del, Var, Shr, App, Abs } type_e;
/* Node types, values they use
 * Type | uint main                          | uint side
 * -----+------------------------------------+----------------------------------
 * Var  | if being copied, ptr to equivalent | number of references of the node
 * Shr  | ptr to term being shared           | number of references of the node
 * App  | ptr to function                    | ptr to argument
 * Abs  | ptr to body of function            | ptr to bound variable
 * Del  | ptr to next node in a ll stack     | extra value for algorithms
 */

// Single node of an LC tree
typedef struct Node {
    type_e type;
    uint main;
    uint side;
} node_t;

// Stores for all nodes of an LC tree
typedef struct Store {
    uint size;     // Number of nodes in store
    uint free;     // Pointer to start of del ll stack
    uint capacity; // Capacity
    node_t *nodes;
} store_t;

// store.c
store_t new_store(uint capacity);
void push_stack(store_t *store, uint *pointer, uint node);
uint pop_stack(store_t *store, uint *pointer);
uint new_node(store_t *store, type_e type, uint main, uint side);
uint new_var(store_t *store);
void del_node(store_t *store, uint node);
void del_tree(store_t *store, uint root);
uint copy_tree(store_t *store, uint root);

//io.c
void print_raw(store_t *store);
void print_tree(store_t *store, uint root);

// reduction.c
uint share_reduce(store_t *store, uint shr);
uint beta_reduce(store_t *store, uint app);
uint left_reduce(store_t *store, uint node);

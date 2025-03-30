#include "ALCI.h"
#include <stdlib.h>

// Create new store
store_t new_store(uint capacity) {
    store_t store;
    
    store.nodes = malloc(sizeof(node_t) * capacity);
    if (store.nodes == NULL)
        return store;
    
    store.capacity = capacity;
    store.size = 0;
    
    for (uint i = 0; i < capacity-1; ++i) {
        store.nodes[i].main = i+1;
    }
    store.nodes[capacity].main = -1;
    return store;
}
// Add new node into the store
uint new_node(store_t *store, type_e type, uint main, uint side) {
    uint index = store->free;
    store->free = store->nodes[index].main;
    store->nodes[index] = (node_t){type, main, side};
    ++store->size;
    return index;
}
// Delete node from store. Doesnt check reference counts
void del_node(store_t *store, uint index) {
    store->nodes[index].main = store->free;
    store->free = index;
    --store->size;
}
// Removes node from store and potencially all linked nodes
// Uses the deleted nodes as a stack, doesnt need extra memory
void del_tree(store_t *store, uint node) {
    uint stack = -1;
    
    while (1) {
        // While given node is Abs or App, add this node into the stack
        //    it will be deleted on the way back up the stack
        while (store->nodes[node].type == Abs ||
               store->nodes[node].type == App) {
            uint next = store->nodes[node].side;
            store->nodes[node].side = stack;
            stack = node;
            node = next;
        }
        if (store->nodes[node].type == Var) {
            if (--store->nodes[node].side == 0)
                del_node(store, node);
        }
        if (store->nodes[node].type == Shr) {
            if (--store->nodes[node].side == 0) {
                store->nodes[node].side = stack;
                stack = node;
                
            }
        }
        // If stack is empty end loop
        if (stack == (uint)-1)
            return;
        
        node = stack;
        stack = store->nodes[node].side;
        uint next = store->nodes[node].main;
        del_node(store, node);
        node = next;
    }
}
// TODO: Rewrite this to not use recursion
uint copy_tree(store_t *store, uint root) {
    switch(store->nodes[root].type) {
    case Var: {
        uint var = store->nodes[root].main;
        ++(store->nodes[var].side);
        return var;
    }
    case Shr:
        ++(store->nodes[root].side);
        return root;
    case Abs: {
        uint var = store->nodes[root].side;
        uint newvar = new_node(store, Var, -1, 1);
        
        store->nodes[var].main = newvar;
        uint body = copy_tree(store, store->nodes[root].main);
        store->nodes[var].main = var;
        store->nodes[newvar].main = newvar;
        return new_node(store, Abs, body, newvar);
        }
    case App: {
        uint func = copy_tree(store, store->nodes[root].main);
        uint arg = copy_tree(store, store->nodes[root].side);
        return new_node(store, App, func, arg);
    }
    }
    return -1; // This never runs
}

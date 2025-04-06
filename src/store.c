#include "ALCI.h"
#include <stdlib.h>
#include <stdio.h>

// Create new Store object with given capacity
store_t new_store(uint capacity) {
    store_t store;
    
    store.nodes = malloc(sizeof(node_t) * capacity);
    if (store.nodes == NULL)
        return store; // Return empty if allocation fails
    
    store.capacity = capacity;
    store.size = 0;
    
    // Link nodes together to create linked list for node adding
    for (uint i = 0; i < capacity-1; ++i) {
        store.nodes[i].main = i+1;
        store.nodes[i].type = Del;
    }
    store.nodes[capacity-1].type = Del;
    store.nodes[capacity-1].main = -1;
    return store;
}
// Push given node to a given stack
void push_stack(store_t *store, uint *pointer, uint node) {
    store->nodes[node].main = *pointer;
    *pointer = node;
}
// Pop node from a given stack
uint pop_stack(store_t *store, uint *pointer) {
    uint node = *pointer;
    *pointer = store->nodes[node].main;
    return node;
}
// Create new node with given type and values
uint new_node(store_t *store, type_e type, uint main, uint side) {
    ++store->size;
    uint node = pop_stack(store, &store->free);
    store->nodes[node] = (node_t){type, main, side};
    return node;
}
// Create new variable
uint new_var(store_t *store) {
    uint var = new_node(store, Var, -1, 1);
    store->nodes[var].main = var;
    return var;
}
// Delete node from store
void del_node(store_t *store, uint node) {
    --store->size;
    push_stack(store, &store->free, node);
    store->nodes[node].type = Del;
}
// Iterativelly delete tree
//   Method deletes nodes from stack filled in DFS order
void del_tree(store_t *store, uint node) {
    uint stack = new_node(store, Del, -1, node); // First node of stack

    while (stack != -1) {
        node = store->nodes[stack].side;

        switch(store->nodes[node].type) {
            case Del:
            // If node is a variable, decrement
            //    If it has no more references left, delete it from store
            case Var:
            if (--store->nodes[node].side == 0)
                del_node(store, node);
            del_node(store, pop_stack(store, &stack));
            break;
            // If node is a share, decrement
            //    If it has no more references, change value on stack to body
            //    Else pop from stack
            case Shr:
            if (--store->nodes[node].side == 0) {
                uint term = store->nodes[node].main;
                store->nodes[stack].side = term;
                del_node(store, node);
            }
            else {
                del_node(store, pop_stack(store, &stack));
            }
            break;
            // If node is an application or an abstraction, change value on
            //    stack to side value, then push to stack main
            //    use this node as the stack node instead of deleting it
            case App:
            case Abs: {
            uint arg = store->nodes[node].main;
            uint body = store->nodes[node].side;
            
            store->nodes[stack].side = body;
            push_stack(store, &stack, node);
            store->nodes[stack].side = arg;
            }
            break;
        }
    }
}
// Helper function for copy_tree
uint copy_tree_child(store_t *store, uint *stack, uint node) {
    switch(store->nodes[node].type) {
        // Return either original variable or copy, depending on if its bound
        case Var: {
        int copy = store->nodes[node].main;
        ++store->nodes[copy].side;
        return copy;
        }
        // Return share
        case Shr:
        ++store->nodes[node].side;
        return node;

        // For both applications and abstractions create new node, push it
        //     to stack
        case App:
        case Abs: {
        uint copy = new_node(store, store->nodes[node].type, -1, node);
        push_stack(store, stack, copy);
        return copy;
        }
        case Del:
    }
    return -1;
}
// Iteratively copy a tree
//    variables use their main value to point to the appropriate copy;
//    either themselves or a copy, created when variable is reached through an
//    abstraction.
uint copy_tree(store_t *store, uint node) {
    // Initial node. If its a var or share no copying is necesarry
    //    Otherwise create a copy and begin loop
    switch(store->nodes[node].type) {
        case Del:
        case Var:
        case Shr:
        ++store->nodes[node].side;
        return node;
        default:
    }
    
    uint stack = new_node(store, store->nodes[node].type, -1, node);
    uint root = stack; // Save root to return later
    
    while (stack != -1) {
        // Extract node to run from stack
        node = store->nodes[stack].side;
        
        switch(store->nodes[node].type) {
            // If node is a variable, its actually a separate stack for cleanup
            //    for variables that have been copied.
            //    Make them point to themselves again, delete node from stack
            case Var: {
            store->nodes[node].main = node;
            del_node(store, pop_stack(store, &stack));
            break;
            }
            // Application: add both its children to stack using
            //    helper function
            case App: {
            uint copy = pop_stack(store, &stack);
            
            uint arg = store->nodes[node].side;
            uint func = store->nodes[node].main;
            
            store->nodes[copy].side = copy_tree_child(store, &stack, arg);
            store->nodes[copy].main = copy_tree_child(store, &stack, func);
            break;
            }
            // Abstraction: create a copy of bound variable, then
            //    make bound variable point to copy
            case Abs: {
            uint copy = pop_stack(store, &stack);
            
            uint var = store->nodes[node].side;
            uint body = store->nodes[node].main;
            
            uint var_copy = new_var(store); // Create copy variable
            store->nodes[var].main = var_copy; // Bind original to copy

            // Push copy variable to cleanup stack
            push_stack(store, &stack, new_node(store, Var, -1, var));

            // Push body to stack
            store->nodes[copy].main = copy_tree_child(store, &stack, body);
            store->nodes[copy].side = var_copy;
            break;
            }
            default:
        }
    }
    return root;
}

#include "ALCI.h"
#include <stdio.h>
#include <string.h>

void print_raw(store_t *store) {
    printf("Size: %d, Capacity: %d, Free: %d\n", store->size, store->capacity, store->free);
    for (uint i = 0; i < store->capacity; ++i) {
        node_t node = store->nodes[i];
        char name[4];
        
        switch(node.type) {
        case Del:
        printf("%d = Del %d, %d\n", i, node.main, node.side);
        break;
        case Var:
        printf("%d = <%d> [%d]\n", i, node.main, node.side);
        break;
        case Shr:
        printf("%d = <%d> {%d}\n", i, node.main, node.side);
        break;
        case App: strncpy(name, "App", 4);
        printf("%d = (%d %d)\n", i, node.main, node.side);
        break;
        case Abs: strncpy(name, "Abs", 4);
        printf("%d = \\%d.%d\n", i, node.side, node.main);
        break;
        }
    }
}

void print_tree_node(store_t *store, uint node);

void print_tree_abs(store_t *store, uint node) {
    printf("\\%d", store->nodes[node].side);
    
    node = store->nodes[node].main;
    
    while (store->nodes[node].type == Abs) {
        printf(" %d", store->nodes[node].side);
        node = store->nodes[node].main;
    }
    printf(".");
    print_tree_node(store, node);
}

void print_tree_node(store_t *store, uint node) {
    switch(store->nodes[node].type) {
        case Del:
        printf("-X-");
        break;
        
        case Var:
        printf("%d", node);
        break;
        
        case Shr:
        printf("{%d}", node);
        break;
        
        case Abs:
        print_tree_abs(store, node);
        break;
        
        case App:
        uint func = store->nodes[node].main;
        
        if (store->nodes[func].type == Abs) {
            printf("(");
            print_tree_node(store, func);
            printf(")");
        }
        else
            print_tree_node(store, func);
        
        printf(" ");
        
        uint arg = store->nodes[node].side;
        
        if (store->nodes[arg].type == App || store->nodes[arg].type == Abs) {
            printf("(");
            print_tree_node(store, arg);
            printf(")");
        }
        else
            print_tree_node(store, arg);
    }
}
void print_tree(store_t *store, uint root) {
    print_tree_node(store, root);
    printf("\n");
    for (int i = 0; i < store->capacity; ++i) {
        if (store->nodes[i].type == Shr) {
            printf("  %d: ", i);
            print_tree_node(store, store->nodes[i].main);
            printf("\n");
        }
    }
}

#include "ALCI.h"
#include <stdio.h>
#include <string.h>

void print_raw(store_t *store) {
    printf("Size: %d, Capacity: %d, Free: %d\n", store->size, store->capacity, store->free);
    for (uint i = 0; i < store->capacity; ++i) {
        node_t node = store->nodes[i];
        char name[4];
        
        switch(node.type) {
        case Var: strncpy(name, "Var", 4); break;
        case Shr: strncpy(name, "Shr", 4); break;
        case App: strncpy(name, "App", 4); break;
        case Abs: strncpy(name, "Abs", 4); break;
        }
        printf("%d = [%s, %d, %d]\n",i , name, node.main, node.side);
    }
}

void print_tree(store_t *store, uint root) {
    switch(store->nodes[root].type) {
    case Var:
        printf("%d[%d]", root, store->nodes[root].side);
        break;
    case Shr:
        printf("%d{%d}", root, store->nodes[root].side);
        break;
    case Abs:
        printf("(\\%d.", store->nodes[root].side);
        print_tree(store, store->nodes[root].main);
        printf(")");
        break;
    case App:
        printf("(");
        print_tree(store, store->nodes[root].main);
        printf(" ");
        print_tree(store, store->nodes[root].side);
        printf(")");
    }
}

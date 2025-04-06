#include "ALCI.h"

uint share_reduce(store_t *store, uint shr) {
    if (--store->nodes[shr].side == 0) {
        uint body = store->nodes[shr].main;
        del_node(store, shr);
        return body;
    }
    return copy_tree(store, store->nodes[shr].main);
}
uint beta_reduce(store_t *store, uint app) {
    uint abs = store->nodes[app].main;
    uint arg = store->nodes[app].side;
    uint var = store->nodes[abs].side;
    uint body = store->nodes[abs].main;
    
    if (--(store->nodes[var].side) == 0) {
        del_tree(store, arg);
        del_node(store, var);
    }
    else {
        store->nodes[var].main = arg;
        store->nodes[var].type = Shr;
    }
    del_node(store, abs);
    del_node(store, app);
    return body;
}
uint left_reduce_app(store_t *store, uint node);
uint left_reduce(store_t *store, uint node) {
    switch(store->nodes[node].type) {
    case Var:
        return node;
    case Shr:
        store->nodes[node].main = left_reduce(store, store->nodes[node].main);
        node = share_reduce(store, node);
        return node;
    case App:
        return left_reduce_app(store, node);
    case Abs:
        store->nodes[node].main = left_reduce(store, store->nodes[node].main);
        return node;
    }
    return -1;
}
uint left_reduce_app(store_t *store, uint node) {
    switch(store->nodes[store->nodes[node].main].type) {
    case Var:
        return node;
    case Shr:
        store->nodes[node].main = left_reduce(store, store->nodes[node].main);
        return left_reduce(store, node);
    case App:
        store->nodes[node].main = left_reduce_app(store, store->nodes[node].main);
        return left_reduce(store, node);
    case Abs:
        node = beta_reduce(store, node);
        return left_reduce(store, node);
    }
    return -1;
}

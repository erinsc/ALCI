#include "ALCI.h"
#include <stdio.h>

int main(void) {
    /*
    store_t store = new_store(16);

    uint varA = new_node(&store, Var, -1, 3);
    uint appA = new_node(&store, App, varA, varA);
    uint absA = new_node(&store, Abs, appA, varA);
    
    uint varB = new_node(&store, Var, -1, 2);
    uint absB = new_node(&store, Abs, varB, varB);
    
    uint app1 = new_node(&store, App, absA, absB);
    
    uint copy = copy_tree(&store, app1);
    del_tree(&store, app1);
    
    print_raw(&store);
    print_tree(&store, copy); printf("\n");
    
    uint reduced = left_reduce(&store, copy);
    
    print_raw(&store);
    print_tree(&store, reduced); printf("\n");
    */
    
    store_t store = new_store(32);
    
    uint varT1 = new_node(&store, Var, 0, 2);
    uint varT2 = new_node(&store, Var, 1, 1);
    uint absT2 = new_node(&store, Abs, varT1, varT2);
    uint absT1 = new_node(&store, Abs, absT2, varT1);
    
    uint varF1 = new_node(&store, Var, 4, 2);
    uint varF2 = new_node(&store, Var, 5, 1);
    uint absF2 = new_node(&store, Abs, varF1, varF2);
    uint absF1 = new_node(&store, Abs, absF2, varF1);
    
    uint and1 = new_node(&store, Var, 8, 3);
    uint and2 = new_node(&store, Var, 9, 2);
    uint andapp2 = new_node(&store, App, and1, and2);
    uint andapp1 = new_node(&store, App, andapp2, and1);
    uint andabs2 = new_node(&store, Abs, andapp1, and2);
    uint andabs1 = new_node(&store, Abs, andabs2, and1);
    
    uint app2 = new_node(&store, App, andabs1, absT1);
    uint app1 = new_node(&store, App, app2, absF1);
    
    uint root = app1;
    
    printf("\n"); print_tree(&store, 15);

    beta_reduce(&store, 14);
    store.nodes[15].main = 12;
    
    printf("\n"); print_tree(&store, 15);
    
    beta_reduce(&store, 15);

    printf("\n"); print_tree(&store, 11);
    
    share_reduce(&store, 8);
    //printf("copy root: %d\n", copy_tree(&store, 11));
    store.nodes[10].main = 15;
    
    printf("\n"); print_tree(&store, 11);

    beta_reduce(&store, 10);
    store.nodes[11].main = 13;
    
    printf("\n"); print_tree(&store, 11);
    
    beta_reduce(&store, 11);
    
    printf("\n"); print_tree(&store, 12);
    
    share_reduce(&store, 12);
    
    printf("\n"); print_tree(&store, 9);
    
    share_reduce(&store, 9);
    
    printf("\n"); print_tree(&store, 7);
}

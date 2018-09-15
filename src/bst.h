#ifndef __BST_H
#define __BST_H

struct bst {
    int _elemsize;
    struct bst_node * root;
    void (*insert)(struct bst * self, void * data);
    void (*remove)(struct bst * self, void * data);
    void (*search)(struct bst * self, void * data);
    void (*balance)(struct bst * self); //use dws algorithim to rebalance tree perfectly
    void * (*greater_than)(void * elem1, void * elem2); //custom comparison for determing which element is larger
 //   void * (*equal_to)(void * elem1, void * elem2); //custom comparison to check if elements are the same
    //Transverse the tree in level order and call given function on each element.
    // Also create a function that does the same in a depth-first way (later)
    void (*level_order)(struct bst * self, void (*func)(struct bst_node * node)); 
      
};

struct bst_node {
    void * data;
    int count; //How many of these nodes are in the tree, starts at 1. If another one is inserted the count is incremented
    struct bst_node * left;
    struct bst_node * right;
};

struct bst bst_new(int elemsize, void * (*greater_than)(void * elem1, void * elem2));
void bst_delete(struct bst * self);


#endif
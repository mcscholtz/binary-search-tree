#include "suite.h"
#include "bst.h"
#include <stdio.h>

void * comp(void * elem1, void * elem2);

void * comp(void * elem1, void * elem2)
{
    printf("%d > %d\n",*(int *)elem1, *(int *)elem2);
    if((*(int *)elem1) > (*(int *)elem2)){
         printf("-->%d\n",*(int *)elem1);
        return elem1;
    }
    else if((*(int *)elem2) > (*(int *)elem1)){
        printf("-->%d\n",*(int *)elem2);
        return elem2;
    }else{
        printf("-->%d==%d\n",*(int *)elem1, *(int *)elem2);
        return NULL; //equal
    }
}

void print_data(struct bst_node * node);
void print_data(struct bst_node * node)
{
    printf("%d(%d) ", *(int *)node->data, node->count);
}

void bst_insert_test(CuTest * tc)
{
    struct bst tree = bst_new(sizeof(int), comp );
    CuAssertPtrEquals(tc, NULL, tree.root);
    int val = 5;
    tree.insert(&tree,&val);
    val = 2;
    tree.insert(&tree,&val);
    val = 3;
    tree.insert(&tree,&val);
    val = 1;
    tree.insert(&tree,&val);
    tree.insert(&tree,&val);
    val = 4;
    tree.insert(&tree,&val);

    tree.level_order(&tree, print_data);

    bst_delete(&tree);
}


int main(void) { return execute(bst_insert_test);}
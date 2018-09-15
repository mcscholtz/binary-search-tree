#include "suite.h"
#include "bst.h"

void * comp(void * elem1, void * elem2);

void * comp(void * elem1, void * elem2)
{
    return *(int *)elem1 > *(int *)elem2 ? elem1: elem2;
}

void bst_create_test(CuTest * tc)
{
    struct bst tree = bst_new(sizeof(int), comp);
    CuAssertPtrEquals(tc, NULL, tree.root);

    bst_delete(&tree);
}


int main(void) { return execute(bst_create_test);}
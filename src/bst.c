#include "bst.h"
#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

//Internal Functions
struct bst_node * bst_create_node(struct bst * self, void * data);
void bst_insert_node(struct bst * self, void * data, struct bst_node * root);
void bst_remove(struct bst * self, struct bst_node * root, struct bst_node * prev, void * data);
struct bst_node * max(struct bst_node * root, struct bst_node * new_root);

//Methods
void bst_insert(struct bst * self, void * data);
void bst_level_order(struct bst * self, void (*func)(struct bst_node * node));
void bst_delete_node(struct bst * self, void * data);

struct bst bst_new(int elemsize, void * (*greater_than)(void * elem1, void * elem2))
{
    struct bst self;
    self.root = NULL;
    self._elemsize = elemsize;
    self.greater_than = greater_than;
    self.insert = bst_insert;
    self.level_order = bst_level_order;
    self.remove = bst_delete_node;
    return self;
}

void bst_delete(struct bst * self)
{
    //TODO: delete all the nodes here that are linked to the root node
}

struct bst_node * bst_create_node(struct bst * self, void * data)
{
    struct bst_node * node = malloc(sizeof(struct bst_node));
    assert(node != NULL && "Out of memory");
    node->data = malloc(self->_elemsize);
    assert(node->data != NULL && "Out of memory");
    memcpy(node->data, data, self->_elemsize);
    node->left = NULL;
    node->right = NULL;
    node->count = 1;
    return node;
}

void bst_insert_node(struct bst * self, void * data, struct bst_node * root)
{
    if(root == NULL){
        root =  bst_create_node(self, data);
        return;
    }
    //printf(" root: %d, node: %d\n", *(int *)root->data, *(int *)data);
    //use compare function
    void * greater = self->greater_than(data, root->data);
    if(greater == root->data){
        //printf("root is greater, go left\n");
        //root is greater
        if(root->left == NULL){
            //printf("inserted to left of root\n");
            root->left = bst_create_node(self, data);
            return;
        }else{
            bst_insert_node(self, data, root->left);
            return;
        }
    }else if(greater == data){
        //printf("node is greater, go right\n");
        if(root->right == NULL){
            root->right = bst_create_node(self, data);
            //printf("inserted to right of root\n");
            return;
        }else{
            bst_insert_node(self, data, root->right);
            return;
        }
    }else{
        //the nodes are equal, increment the root node's count, dont create another node
        root->count++;
    }
}

void bst_level_order(struct bst * self, void (*func)(struct bst_node * node))
{
    struct queue * q = queue_new(sizeof(struct bst_node));
    assert(self->root != NULL);

    struct bst_node root;
    q->enqueue(q, self->root);
    while(q->length > 0){
        //dequeue first item
        q->dequeue(q, &root);
        
        //call custom function on the data
        func(&root);
        
        //push children
        if(root.left != NULL){
            q->enqueue(q, root.left);
        }
        if(root.right != NULL){
            q->enqueue(q, root.right);
        }
    }
    queue_delete(q);
}

void bst_insert(struct bst * self, void * data)
{
    if(self->root == NULL){
        //insert the root node
        self->root = bst_create_node(self, data);
        return;
    }
    //Insert
    bst_insert_node(self, data, self->root);
}

void bst_find(struct bst * self, void * data)
{
    struct bst_node * node = self->root;
}

//using the equal_to function find the matching element and delete it
void bst_delete_node(struct bst * self, void * data)
{
    //recursive call to walk down tree, delete the node (or decrease counter by 1) then return
    bst_remove(self, self->root, NULL, data);
}

//use const pointers
struct bst_node * max(struct bst_node * root, struct bst_node * new_root)
{
    printf("called max\n");
    printf("  addr(root): %x\n", root);
    struct bst_node * prev = NULL;
    struct bst_node * current = root;
    while(current->right != NULL){
        prev = current;
        current = current->right;
    }

    printf("found max: %d\n", *(int *)current->data);

    //max = current;

    if(prev == NULL){
        printf("max->left is new root\n");
        printf("new root: %d\n", *(int *)current->left->data);
        printf("  addr(new_root): %x\n", new_root);
        new_root = current->left;
        assert(new_root == current->left);
        printf("new root: %d\n", *(int *)new_root->data);
        return current;
    }else{
        new_root = root;
        if(current->left == NULL){
            prev->right = NULL;
        }else{
            //node on left
            prev->right = current->left;
        }
        return current;
    }
}

void bst_remove(struct bst * self, struct bst_node * root, struct bst_node * prev, void * data)
{
    //printf("call remove\n");
    void * greater = self->greater_than(data, root->data);
    if(greater == data){
        //walk to right
        //printf("walk right\n");
        bst_remove(self, root->right, root, data);
        return;
    }else if(greater == root->data){
        //walk to left
        //printf("walk left\n");
        bst_remove(self, root->left, root, data);
        return;
    }else{
        //this is the node to remove
        //printf("found match\n");
        if(root->count > 1){
            //printf("case 0\n");
            root->count--;
            return;
        }else{
            //remove the node.
            //case 1: both leaves are NULL, easy just remove it, replace with NULL
            if(root->left == NULL && root->right == NULL){
                //printf("case 1\n");
                if(prev != NULL){
                    if(prev->right == root){
                        prev->right = NULL;
                    }else{
                        prev->left = NULL;
                    }
                }
                free(root->data);
                free(root);
                return; 
            }
            //case 2: one of the leaves is NULL, delete and replace with non-NULL leaf
            if(root->left == NULL){
                //printf("case 2a\n");
                if(prev == NULL){
                    //root node
                    self->root = root->right;
                    //printf("root is now: ");
                }else{
                    //printf("%d now points to ", *(int *)prev->data);
                    if(prev->right == root){
                        prev->right = root->right;
                    }else if(prev->left == root){
                        prev->left = root->right;
                    }
                } 
                //printf("%d\n", *(int *)root->right->data);
                free(root->data);
                free(root);
                return; 
            }
            if(root->right == NULL){
                //printf("case 2b\n");
                if(prev == NULL){
                    //root node
                    self->root = root->left;
                    //printf("root is now: ");
                }else{
                    //printf("%d now points to ", *(int *)prev->data);
                    if(prev->left == root){
                        prev->left = root->left;
                    }else if(prev->right == root){
                        prev->right = root->left;
                    }
                }
                //printf("%d\n", *(int *)root->left->data);
                free(root->data);
                free(root);
                return; 
            }
            //case 3: both leaves are non-NULL, delete node, replace with minimum in right sub tree or max in left subtree
            //printf("case 3\n");
            //for now just assume max left (but can also do min right)

            struct bst_node * prev_m = NULL;
            struct bst_node * new_root = NULL;
            struct bst_node * current = root->left;
            while(current->right != NULL){
                prev_m = current;
                current = current->right;
            }

            if(prev_m == NULL){
                new_root = current->left;
            }else{
                new_root = root;
                if(current->left == NULL){
                    prev_m->right = NULL;
                }else{
                    //node on left
                    prev_m->right = current->left;
                }
            }
            //replace root with new root and delete root
            root->left = new_root;
            memcpy(root->data, current->data, self->_elemsize);
           
            free(current->data);
            free(current);
        }
    }
}



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/* 
   Trong Le

   Homework 3
   Robert Finn
   Systems 1 10:20
*/

typedef struct Tree {
    void *node;
    int elemSize;
    struct Tree *left, *right, *parent;
} Tree;

/* Traverse through tree, getting right value of T, until reaching NULL terminator */ 
Tree *max(Tree *T) {
    
    while (T->right != NULL) {
        T = T->right;
    }

    return T;
}


/* Traverse through tree, getting left value of T, until reaching NULL terminator */
Tree *min(Tree *T) {

    while (T->left != NULL) {
        T = T->left;
    }

    return T;
}


/* Traverse through until finding appropriate value */
Tree *search(Tree *T, void *keyValue, int (*cmpfn)(void *v1, void *v2)) {

    while (T != NULL && (cmpfn(T->node, keyValue) != 0)) {
        
        if (cmpfn(T->node, keyValue) >= 0) {
            T = T->left;
        } else {
            T = T->right;
        }
    }

    return T;
}


/* Insert nodes in binary tree by creating new/duplicate tree and adding into existing tree */
void insert(Tree **T, void *keyValue, int size, int (*cmpfn)(void *v1, void *v2)) {

    Tree *y = NULL;
    Tree *x = *T;
    Tree *z = malloc(sizeof(Tree));
    

    while (x != NULL) {
        y = x;
        if (cmpfn(x->node, keyValue) >= 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    memcpy(z->node, keyValue, size);
    z->parent = y;
    z->left = NULL;
    z->right = NULL;
    
    if (y != NULL) {
        if (cmpfn(y->node, z->node) >= 0) {
            y->left = z;
        } else {
            y->right = z;
        }
    }
    
    if (*T == NULL) {
        *T = z;
    }
}

/* Int comparison function call */
int IntCmp(void *vp1, void *vp2) {
    int *i1 = vp1;
    int *i2 = vp2;
    return *i1-*i2;
}

/* String comparison function call */
int StrCmp(void *vp1, void *vp2) {
    char *s1 = *(char **)vp1;
    char *s2 = *(char **)vp2;
    return strcmp(s1,s2);
}

/* Print binary tree in ascending order */
void printAsc(Tree *T, char type) {

    if (T->left != NULL) {
        printAsc(T->left, type);
    }

    /* Check types before printing out */
    switch(type) {
        case 'I':
            printf("%d\n", *(int*) T->node);
            break;
        case 'F':
            printf("%f\n", *(float*) T->node);
            break;
        case 'S':
            printf("%s\n", *(char **) T->node);
            break;
    }

    if (T->right != NULL) {
        printAsc(T->right, type);
    }
}


/* Given node, finds pointer to next higher value */
Tree *successor(Tree *key) {

    if (key->right != NULL) { 
        key = min(key->right);
    } else {
        while ((key->parent != NULL) && (key->parent)->left != key) {
            key = key->parent;
        }
        key = key->parent;
    }

    return key;
}

void deleteNode(Tree **root, void *value) {

    /* Find location of node to delete */   /* Check to make sure comparison func call is correct */
    Tree *T = search(*root, value, StrCmp);

    bool t_hasParent = (T->parent != NULL);

    /* Make a dummy parent */
    Tree *dummy;
    if (!t_hasParent) {
        dummy = malloc(sizeof(Tree));
        dummy->left = T;
        T->parent = dummy;
    }
    
    bool t_isRoot = (T == *root);
    bool t_isLeft = ((T->parent)->left == T);

    /* Case 1: Both childrent of Tree T are NULL */
    if (T->left == NULL && T->right == NULL) {
        if ((T->parent)->left == T) {
            (T->parent)->left = NULL;
        } else if ((T->parent)->right == T) {
            (T->parent)->right = NULL;
        }
    }

    /* Case 2: Left child of T not NULL, right child of T NULL */
    else if (T->left != NULL && T->right == NULL) {
        if ((T->parent)->left == T) {
            (T->parent)->left = T->left;
        } else {
            (T->parent)->right = T->left;
        }
        (T->left)->parent = T->parent;
    }

    /* Case 3: Left child of T NULL, right child of T not NULL */
    else if (T->left == NULL && T->right != NULL) {
        if ((T->parent)->left == T) {
            (T->parent)->left = T->right;
        } else {
            (T->parent)->right = T->right;
        }
        (T->right)->parent = T->parent;
    }

    /* Case 4: Both children not NULL, left child of right child NULL */
    else if (T->left != NULL && T->right != NULL && (T->right)->left == NULL) {
        if ((T->parent)->left == T) {
            (T->parent)->left = T->right;
        } else {
            (T->parent)->right = T->right;
        }
        (T->right)->parent = T->parent;
        (T->right)->left = T->left;
    }

    /* Case 5: Both children and it's children are not NULL */
    else if (T->left != NULL && T->right != NULL && (T->right)->left != NULL) {
        Tree *a = min(T->right);
        (a->parent)->left = a->right;
        if (a->right != NULL) {
            (a->right)->parent = a->parent;
        }
        (T->right)->parent = a;
        (T->left)->parent = a;
        a->parent = T->parent;
        a->left = T->left;
        a->right = T->right;
        if ((T->parent)->left == T) {
            (T->parent)->left = a;
        } else {
            (T->parent)->right = a;
        }
    }
    
    /* Case for if T is root */
    if (t_isRoot) {
        if (t_isLeft) {
            *root = (T->parent)->left;
        } else {
            *root = (T->parent)->right;
        }
    }

    /* Case for if T has no parent */
    if (!t_hasParent) {
        (dummy->left)->parent = NULL;
        free(dummy);
    }

    free(T->node);
    free(T);
}

int main(void) {
    
    Tree *T;
    /*insert(&T, 4);
    insert(&T, 10);
    insert(&T, 2);
    insert(&T, 9);
    insert(&T, 1);
    insert(&T, 8);
    insert(&T, 3); */

    //deleteNode(&T, 1);

    printAsc(T, 'I');
}

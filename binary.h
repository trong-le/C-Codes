#ifndef BINARY_H_
#define BINARY_H_

/* Declare tree type and struct */
typedef struct Tree {
    int key;
    struct Tree *left, *right, *parent;
} Tree;

/* Declare function prototypes */
Tree *max(Tree *T);
Tree *min(Tree *T);
Tree *search(Tree *T, int keyValue);
void insert(Tree **T, int keyValue); 
void printAsc(Tree *T);
Tree *successor(Tree *node);
void deleteTree(Tree **root, int value);

#endif

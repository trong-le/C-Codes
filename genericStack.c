#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct{
    void *elems;
    int logLen;
    int allocLen;
    int elemSize;
} stack;

void stackInit(stack *s, int size) {
    s->elemSize = size;
    s->logLen = 0;
    s->allocLen = 4;
    s->elems = malloc(4*size);
    assert(s->elems != NULL);
}

void stackDel(stack *s) {
    free(s->elems);
}

void stackPush(stack *s, void *addr) {
    if (s->logLen == s->allocLen) {
        s->allocLen *= 2;
        s->elems = realloc(s->elems, s->allocLen*s->elemSize);
        assert(s->elems != NULL);
    }
    void *target = (char *)s->elems + s->logLen*s->elemSize;
    memcpy(target, addr, s->elemSize);
    s->logLen++;
}

void stackPop(stack *s, void *elem) {
    assert(s->logLen != 0);
    s->logLen--;
    void *source = (char *)s->elems + s->logLen*s->elemSize;
    memcpy(elem, source, s->elemSize);
}

int main() {
    
    int i;
    char *names[] = {"Chuck", "Dave", "Ellen"};
    stack stringStack;
    stackInit(&stringStack, sizeof(char*));
    for (i=0; i<3; i++) {
        char *copy = strdup(names[i]);
        stackPush(&stringStack, &copy);
    }

    for (i=0; i<3; i++) {
        char *name;
        stackPop(&stringStack, &name);
        printf("%s\n", name);
        free(name);
    }

    stackDel(&stringStack);
}

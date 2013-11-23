tree
====

A wrapper around sys/tree.h.

## Example

The following program reads lines from ```stdin```, inserts them into a
red-black tree, and on ```EOF``` prints them to ```stdout``` in-order.

```c
#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node {
    char* line;
    RB_ENTRY(node) entry;
};

struct node* node_new(char* string);
int node_cmp(struct node* a, struct node* b);

RBTREE_DEFINE(tree, node, entry, node_cmp);

struct node*
node_new(char* line)
{
    struct node* n = calloc(1, sizeof(struct node));
    n->line = strdup(line);
    return n;
}

int
node_cmp(struct node* a, struct node* b)
{
    return strcmp(a->line, b->line);
}

int
main(int argc, char const* argv[])
{
    struct tree t;
    size_t len = 0;
    char* line = NULL;
    
    tree_init(&t);
    
    while (getline(&line, &len, stdin) != -1) {
        struct node query = { line };
        if (!tree_find_node(&t, &query))
            tree_insert_node(&t, node_new(line));
    }
    
    printf("Sorted lines:\n");
    tree_each(&t, node, n, {
        printf("%s", n->line);
    });
    
    return 0;
}
```

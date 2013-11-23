Tree
====

A wrapper around sys/tree.h.

## Usage

### Red-black trees

Macro ```RBTREE_DEFINE(name, type, field, cmp)``` defines a red-black tree 
with type *name* and its operations. *type* is the name of the structure that 
represents the nodes of the tree, and are linked through a user defined field 
*field*. Finally, *cmp* is a function pointer of type 
```int (*cmp)(struct type*)```.

Macro ```RB_ENTRY(type)``` defines a structure that contains the links to the 
left, right and parent nodes.

The example below shows how to declare a red-black tree containing integer keys.

```c
struct node {
    int key;
    RB_ENTRY(node) entry;
};

int node_cmp(struct node* n);
RBTREE_DEFINE(tree, node, entry, cmp);
```

### Splay trees

Same as above, except that the macros are ```SPLAY_ENTRY()``` and ```SPLAYTREE_DEFINE()```.


### API

#### tree\_init\_*type*(struct *name* \*t);

#### tree\_insert\_*type*(struct *name* \*t, struct *type* \*e);

#### tree\_remove\_*type*(struct *name* \*t, struct *type* \*e);

#### tree\_find\_*type*(struct *name* \*t, struct *type* \*e);

#### tree\_min\_*type*(struct *name* \*t);

#### tree\_max\_*type*(struct *name* \*t);

#### tree\_next\_*type*(struct *name* \*t, struct *type* \*e);

#### tree\_apply\_*type*(struct *name* \*t, struct *type* \*e);

#### tree\_destroy\_*type*(struct *name* \*t, struct *type* \*e);

#### tree\_each(*head*,  *type*, *iter*, *block*);

```c
struct tree t;
tree_each(&t, node, n, {
    printf("%d", n->key);
});
```

#### tree\_each\_safe(*head*,  *type*, *iter*, *block*);


## Example

The following program reads lines from ```stdin```, inserts them into a
red-black tree, and on ```EOF``` prints them to ```stdout``` in order.

```c
#include "tree.h"

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

    tree_init_node(&t);

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

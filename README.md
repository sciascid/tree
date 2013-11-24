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

```RBTREE_DEFINE(tree, node, entry, cmp)``` or the equivalent 
```SPLAYTREE_DEFINE``` generate the following API:

```c
void tree_init_node(struct tree *t);
```

Initializes an empty tree ```t```.
***

```c
struct node* tree_insert_node(struct tree* t, struct node* e);
```

Inserts node ```e``` in tree ```t```.
***

```c
struct node* tree_remove_node(struct tree *t, struct node *e);
```

Removes the first node in tree ```t```, that matches node ```e```, according 
to the comparator function. Returns the removed node, or ```NULL```.
***

```c 
struct node* tree_find_node(struct tree* t, struct node* e);
```

Returns a pointer to the first node that matches ```e```, or ```NULL```.
***    

```c
struct node* tree_min_node(struct tree* t);
```

Returns the smallest node in ```t```, or ```NULL``` if the ```t``` is empty.
***


```c
struct node* tree_max_node(struct tree* t);
```

Returns the largest node in ```t```, or ```NULL``` if the ```t``` is empty.
***

```c 
struct node* tree_next_node(struct tree* t, struct node* e);
```

Returns the next node in tree ```t``` larger than ```e```, or ```NULL```.
***

```c
void tree_apply_node(struct tree* t, void(*cb)(struct node*));
```

Calls the given function ```cb``` on every node in tree ```t```.
***

```c
void tree_destroy_node(struct tree* t, void(*free_cb)(struct node*));
```

Removes all nodes in the given tree ```t``` and frees their memory by calling
the given function ```free_cb```.
***

```c 
void tree_each(name, type, iter, block);
```

```tree_each``` is a macro that iterates over all nodes in the tree.

```c
struct tree t;
tree_each(&t, node, n, {
    printf("%d", n->key);
});
```
***

```c
void tree_each_safe(name, type, iter, block);
```
Same as ```tree_each```, except that it is same to remove node ```iter``` from
the tree while iterating.


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

/*
 * Copyright (c) 2013, Daniele Sciascia
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holders nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _TREE_H_
#define _TREE_H_

/*
 *  A wrapper around BSD's sys/tree.h.
 */

#include "sys_tree.h"

#define RBTREE_DEFINE(name, type, field, cmp) \
    TREE_DEFINE(RB, name, type, field, cmp, __unused static inline);

#define SPLAYTREE_DEFINE(name, type, field, cmp) \
    TREE_DEFINE(SPLAY, name, type, field, cmp, __unused static inline);

#define TREE_DEFINE(TREE, name, type, field, cmp, attr)                     \
    TREE##_HEAD(name, type);                                                \
    TREE##_PROTOTYPE_STATIC(name, type, field, cmp);                        \
    TREE##_GENERATE_STATIC(name, type, field, cmp);                         \
    attr void                                                               \
    tree_init_##type(struct name* t)                                        \
    {                                                                       \
        TREE##_INIT(t);                                                     \
    }                                                                       \
    attr int                                                                \
    tree_empty_##type(struct name* t)                                       \
    {                                                                       \
        return TREE##_EMPTY(t);                                             \
    }                                                                       \
    attr struct type*                                                       \
    tree_insert_##type(struct name* t, struct type* e)                      \
    {                                                                       \
        return TREE##_INSERT(name, t, e);                                   \
    }                                                                       \
    attr struct type*                                                       \
    tree_remove_##type(struct name* t, struct type* e)                      \
    {                                                                       \
        return TREE##_REMOVE(name, t, e);                                   \
    }                                                                       \
    attr struct type*                                                       \
    tree_find_##type(struct name* t, struct type* e)                        \
    {                                                                       \
        return TREE##_FIND(name, t, e);                                     \
    }                                                                       \
    attr struct type*                                                       \
    tree_min_##type(struct name* t)                                         \
    {                                                                       \
        return TREE##_MIN(name, t);                                         \
    }                                                                       \
    attr struct type*                                                       \
    tree_max_##type(struct name* t)                                         \
    {                                                                       \
        return TREE##_MAX(name, t);                                         \
    }                                                                       \
    attr struct type*                                                       \
    tree_next_##type(struct name* t, struct type* e)                        \
    {                                                                       \
        (void)t; /* t is not used by RB_NEXT */                             \
        return TREE##_NEXT(name, t, e);                                     \
    }                                                                       \
    attr void                                                               \
    tree_apply_##type(struct name* t, void(*cb)(struct type*))              \
    {                                                                       \
        tree_each(t, type, e, cb(e));                                       \
    }                                                                       \
    attr void                                                               \
    tree_destroy_##type(struct name* t, void(*free_cb)(struct type*))       \
    {                                                                       \
        tree_each_safe(t, type, e, free_cb(tree_remove_##type(t, e)));      \
    }

#define tree_each(t, type, e, block)                                        \
    {                                                                       \
        struct type* e;                                                     \
        for (e = tree_min_##type(t);                                        \
             e != NULL;                                                     \
             e = tree_next_##type(t, e)) {                                  \
            block;                                                          \
        }                                                                   \
    }

#define tree_each_safe(t, type, e, block)                                   \
    {                                                                       \
        struct type *e, *__tmp;                                             \
        for (e = tree_min_##type(t);                                        \
             (e != NULL) && (__tmp= tree_next_##type(t, e), e != NULL);     \
             e = __tmp) {                                                   \
            block;                                                          \
        }                                                                   \
    }

#endif

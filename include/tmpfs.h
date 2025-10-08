#ifndef _TMPFS_H
#define _TMPFS_H

#include "fs.h"

#define MAX_CHILDREN 16

enum tmpfs_type {
    TMPFS_FILE,
    TMPFS_DIR,
};

struct tmpfs_node {
    struct vnode vnode;
    enum tmpfs_type type;
    struct tmpfs_node* children[MAX_CHILDREN];
    int num_children;
    char* content;
    int content_size;
};

int tmpfs_init(struct filesystem* fs);

#endif

#include "tmpfs.h"
#include "string.h"
#include "mm.h"
#include "printf.h"

static int tmpfs_lookup(struct vnode* dir, struct vnode** target, const char* component_name) {
    struct tmpfs_node* dir_node = (struct tmpfs_node*)dir->internal;
    for (int i = 0; i < dir_node->num_children; i++) {
        if (strcmp(dir_node->children[i]->vnode.name, component_name) == 0) {
            *target = &dir_node->children[i]->vnode;
            return 0;
        }
    }
    return -1;
}

static int tmpfs_create(struct vnode* dir, struct vnode** target, const char* component_name, enum vfs_type type) {
    struct tmpfs_node* dir_node = (struct tmpfs_node*)dir->internal;
    if (dir_node->num_children >= MAX_CHILDREN) {
        return -1; // directory full
    }

    struct tmpfs_node* new_node = (struct tmpfs_node*)kmalloc(sizeof(struct tmpfs_node));
    strcpy(new_node->vnode.name, component_name);
    new_node->type = (type == VFS_FILE) ? TMPFS_FILE : TMPFS_DIR;
    new_node->num_children = 0;
    new_node->content = NULL;
    new_node->content_size = 0;
    new_node->vnode.parent = dir;
    new_node->vnode.internal = new_node;
    new_node->vnode.fs = dir->fs; // set the fs

    dir_node->children[dir_node->num_children++] = new_node;
    *target = &new_node->vnode;
    return 0;
}

static int tmpfs_write(struct file* file, const void* buf, unsigned int len) {
    struct tmpfs_node* node = (struct tmpfs_node*)file->vnode->internal;
    if (node->type == TMPFS_DIR) {
        return -1; // cant write to a directory
    }

    if (node->content) {
        kfree(node->content);
    }

    node->content = kmalloc(len);
    if (!node->content) {
        return -1; // out of memory
    }

    memcpy(node->content, buf, len);
    node->content_size = len;
    return len;
}

static int tmpfs_read(struct file* file, void* buf, unsigned int len) {
    struct tmpfs_node* node = (struct tmpfs_node*)file->vnode->internal;
    if (node->type == TMPFS_DIR) {
        return -1; // cant read from a directory
    }

    unsigned int read_size = len > node->content_size ? node->content_size : len;
    if (read_size > 0) {
        memcpy(buf, node->content, read_size);
    }
    return read_size;
}


static int tmpfs_setup_mount(struct filesystem* fs, struct vnode** root) {
    struct tmpfs_node* root_node = (struct tmpfs_node*)kmalloc(sizeof(struct tmpfs_node));
    strcpy(root_node->vnode.name, "/");
    root_node->vnode.parent = &root_node->vnode; // roots parent is itself
    root_node->type = TMPFS_DIR;
    root_node->num_children = 0;
    root_node->vnode.internal = root_node;
    root_node->vnode.fs = fs; // set the filesystem
    *root = &root_node->vnode;
    return 0;
}

int tmpfs_init(struct filesystem* fs) {
    fs->name = "tmpfs";
    fs->setup_mount = tmpfs_setup_mount;
    fs->lookup = tmpfs_lookup;
    fs->create = tmpfs_create;
    fs->read = tmpfs_read;
    fs->write = tmpfs_write;
    printf("tmpfs initialized\n");
    return 0;
}

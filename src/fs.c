#include "fs.h"
#include "tmpfs.h"
#include "printf.h"
#include "string.h"
#include "mm.h"

static struct filesystem tmpfs;
static struct vnode* root_vnode;

void vfs_init() {
    tmpfs_init(&tmpfs);
    tmpfs.setup_mount(&tmpfs, &root_vnode);
    printf("VFS initialized.\n");
}

struct vnode* vfs_get_root() {
    return root_vnode;
}

// helper function to get the next component from a path
static const char* next_component(const char* path, char* component) {
    int i = 0;
    while (*path && *path == '/') {
        path++;
    }
    while (*path && *path != '/') {
        component[i++] = *path++;
    }
    component[i] = '\0';
    return path;
}


int vfs_lookup(const char* path, struct vnode** node) {
    struct vnode* current = root_vnode;
    char component[MAX_FILENAME_LEN];
    const char* path_ptr = path;

    while (*(path_ptr = next_component(path_ptr, component))) {
        if (component[0] == '\0') {
            break;
        }
        if (tmpfs.lookup(current, &current, component) != 0) {
            return -1; // not found
        }
    }

    *node = current;
    return 0;
}

int vfs_create(const char* path, int type) {
    char parent_path[MAX_PATH_LEN];
    const char* new_name;
    struct vnode* parent_node;

    // find the last '/' to separate parent path and new name
    const char* last_slash = strrchr(path, '/');
    if (last_slash == NULL) {
        // no slash, creating in root
        strcpy(parent_path, "/");
        new_name = path;
    } else {
        int parent_len = last_slash - path;
        if (parent_len == 0) {
            strcpy(parent_path, "/");
        } else {
            strncpy(parent_path, path, parent_len);
            parent_path[parent_len] = '\0';
        }
        new_name = last_slash + 1;
    }

    if (vfs_lookup(parent_path, &parent_node) != 0) {
        return -1; // parent not found
    }

    struct vnode* new_node;
    return tmpfs.create(parent_node, &new_node, new_name, (enum vfs_type)type);
}

int vfs_open(const char* path, int flags, struct file** file) {
    struct vnode* node;
    if (vfs_lookup(path, &node) != 0) {
        // for now, we dont create files on open. use vfs_create first
        return -1;
    }

    *file = (struct file*)kmalloc(sizeof(struct file));
    if (!*file) {
        return -1; // memory allocation failed
    }
    (*file)->vnode = node;
    (*file)->flags = flags;
    return 0;
}

int vfs_close(struct file* file) {
    kfree(file);
    return 0;
}

int vfs_read(struct file* file, void* buf, unsigned int len) {
    return tmpfs.read(file, buf, len);
}
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
        if (i < MAX_FILENAME_LEN - 1) {
            component[i++] = *path;
        }
        path++;
    }
    component[i] = '\0';
    return path;
}


int vfs_lookup_at(struct vnode* base, const char* path, struct vnode** node) {
    struct vnode* current = (path[0] == '/') ? root_vnode : base;
    char component[MAX_FILENAME_LEN];
    const char* path_ptr = path;

    while (*path_ptr != '\0') {
        path_ptr = next_component(path_ptr, component);
        if (component[0] == '\0') {
            break;
        }
        if (strcmp(component, ".") == 0) {
            continue;
        }
        if (strcmp(component, "..") == 0) {
            current = current->parent;
            continue;
        }
        if (tmpfs.lookup(current, &current, component) != 0) {
            return -1; // not found
        }
    }

    *node = current;
    return 0;
}

int vfs_lookup(const char* path, struct vnode** node) {
    return vfs_lookup_at(root_vnode, path, node);
}

int vfs_create_at(struct vnode* base, const char* path, int type) {
    char parent_path[MAX_PATH_LEN];
    const char* new_name;
    struct vnode* parent_node;

    // find the last '/' to separate parent path and new name
    const char* last_slash = strrchr(path, '/');
    if (last_slash == NULL) {
        // no slash, creating in base/cwd
        parent_node = base;
        new_name = path;
    } else {
        int parent_len = last_slash - path;
        if (parent_len == 0) {
            parent_node = root_vnode;
        } else {
            if (parent_len >= MAX_PATH_LEN) {
                return -1; // path too long
            }
            strncpy(parent_path, path, parent_len);
            parent_path[parent_len] = '\0';
            if (vfs_lookup_at(base, parent_path, &parent_node) != 0) {
                return -1; // parent not found
            }
        }
        new_name = last_slash + 1;
    }

    if (new_name[0] == '\0') {
        return -1; // empty file name not allowed
    }
    if (strlen(new_name) >= MAX_FILENAME_LEN) {
        return -1; // filename too long
    }

    struct vnode* new_node;
    return tmpfs.create(parent_node, &new_node, new_name, (enum vfs_type)type);
}

int vfs_create(const char* path, int type) {
    return vfs_create_at(root_vnode, path, type);
}

int vfs_open_at(struct vnode* base, const char* path, int flags, struct file** file) {
    struct vnode* node;
    if (vfs_lookup_at(base, path, &node) != 0) {
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

int vfs_open(const char* path, int flags, struct file** file) {
    return vfs_open_at(root_vnode, path, flags, file);
}

int vfs_close(struct file* file) {
    kfree(file);
    return 0;
}

int vfs_read(struct file* file, void* buf, unsigned int len) {
    return tmpfs.read(file, buf, len);
}

int vfs_write(struct file* file, const void* buf, unsigned int len) {
    return tmpfs.write(file, buf, len);
}
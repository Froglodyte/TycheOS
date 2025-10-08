#ifndef _FS_H
#define _FS_H

#define MAX_FILENAME_LEN 256
#define MAX_PATH_LEN 1024

enum vfs_type {
    VFS_FILE,
    VFS_DIR,
};

struct vnode {
    struct vnode* parent;
    char name[MAX_FILENAME_LEN];
    struct filesystem* fs;
    void* internal;
};

struct file {
    struct vnode* vnode;
    unsigned int flags;
};

struct filesystem {
    const char* name;
    int (*setup_mount)(struct filesystem* fs, struct vnode** root);
    int (*lookup)(struct vnode* dir, struct vnode** target, const char* component_name);
    int (*create)(struct vnode* dir, struct vnode** target, const char* component_name, enum vfs_type type);
    int (*read)(struct file* file, void* buf, unsigned int len);
    int (*write)(struct file* file, const void* buf, unsigned int len);
};

void vfs_init();
struct vnode* vfs_get_root();
int vfs_lookup(const char* path, struct vnode** node);
int vfs_create(const char* path, int type);
int vfs_open(const char* path, int flags, struct file** file);
int vfs_close(struct file* file);
int vfs_read(struct file* file, void* buf, unsigned int len);
int vfs_write(struct file* file, const void* buf, unsigned int len);


#endif

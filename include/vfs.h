#ifndef VFS_H
#define VFS_H

#include <stdint.h>

#define MAX_FILES 16
#define MAX_FILENAME_LEN 32
#define MAX_FILE_CONTENT_LEN 256

typedef struct {
    char name[MAX_FILENAME_LEN];
    char content[MAX_FILE_CONTENT_LEN];
    uint32_t size;
} vfs_file_t;

void vfs_init();
int vfs_create(const char* filename);
int vfs_write(const char* filename, const char* data);
const char* vfs_read(const char* filename);
void vfs_ls();

#endif

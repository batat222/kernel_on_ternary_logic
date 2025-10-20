#ifndef FS_H
#define FS_H

void fs_init(void);
void fs_list(const char *path);
char *fs_read(const char *path);

#endif


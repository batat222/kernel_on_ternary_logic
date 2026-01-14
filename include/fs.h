#ifndef FS_H
#define FS_H

void fs_list(const char *path);
int  fs_cd(const char *path);
int  fs_copy(const char *src, const char *dst);
int  fs_move(const char *src, const char *dst);
int  fs_remove(const char *path);
int  fs_cat(const char *path);

#endif

#include "fs.h"
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

void fs_list(const char *path) {
    if (!path) path = ".";
    DIR *d = opendir(path);
    if (!d) { perror("ls"); return; }
    struct dirent *e;
    while ((e = readdir(d))) printf("%s  ", e->d_name);
    printf("\n");
    closedir(d);
}

int fs_cd(const char *path) {
    if (!path) path = "/";
    if (chdir(path) != 0) { perror("cd"); return -1; }
    return 0;
}

int fs_copy(const char *src, const char *dst) {
    FILE *a = fopen(src, "rb");
    FILE *b = fopen(dst, "wb");
    if (!a || !b) { perror("cp"); return -1; }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), a)))
        fwrite(buf, 1, n, b);
    fclose(a); fclose(b);
    return 0;
}

int fs_move(const char *src, const char *dst) {
    if (rename(src, dst) == 0) return 0;
    if (fs_copy(src, dst) == 0) return fs_remove(src);
    return -1;
}

int fs_remove(const char *path) {
    if (remove(path) != 0) { perror("rm"); return -1; }
    return 0;
}

int fs_cat(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { perror("cat"); return -1; }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)))
        fwrite(buf, 1, n, stdout);
    fclose(f);
    return 0;
}

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "fs.h"

void fs_init(void) {}

/* List directory entries */
void fs_list(const char *path) {
    DIR *d = opendir(path ? path : ".");
    if (!d) { perror("ls"); return; }
    struct dirent *ent;
    while ((ent = readdir(d))) {
        if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
            printf("%s  ", ent->d_name);
    }
    printf("\n");
    closedir(d);
}

/* Read a text file into memory */
char *fs_read(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = 0;
    fclose(f);
    return buf;
}


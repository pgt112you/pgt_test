#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pgt_test.h"


int main() {
    DIR *dirp;
    struct dirent *dp;    
    const char *dirpath = "/home/guangtong/pgt_test/";
    char filepath[256];
    dirp = opendir(dirpath);
    if (dirp == NULL) {
        printf("can not open %s\n", dirpath);
        exit(0);
    }
    while ((dp = readdir(dirp)) != NULL) {
        if ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0)) {
            continue;
        }
        struct stat stbuf;
        bzero(filepath, 256);
        sprintf(filepath, "%s%s", dirpath, dp->d_name);
        if (stat(filepath, &stbuf) < 0) {
            printf("get file %s status failed\n", filepath);
        }
        printf("file %s %d\n", dp->d_name, stbuf.st_mtime);
    }
}

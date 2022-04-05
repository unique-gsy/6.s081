#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define STDIN_FILE_NO 0
#define STDOUT_FILE_NO 1
#define STDERR_FILE_NO 2

char* getname(char* path) {
    static char buf[DIRSIZ + 1];
    char * p;

    for (p = path + strlen(path); p >= path && *p != '/'; --p);
    ++p;

    if (strlen(p) > DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));

    if (strlen(buf) == 0) return buf;
    for (int i = strlen(buf); i > 0; --i) {
        if (buf[i - 1] != ' ') {
            buf[i] = '\0';
            return buf;
        }
    }
    return buf;
}

void find(char* path, char* name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(STDERR_FILE_NO, "ls : cann't open %s\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(STDERR_FILE_NO, "ls : cann't stat %s\n", path);
        close(fd);
        exit(1);
    }

    switch (st.type)
    {
    case T_FILE :
        if (strcmp(name, getname(path)) == 0) {
            fprintf(STDOUT_FILE_NO, "%s\n", getname(path));
        }
        break;
    case T_DIR :
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
            fprintf(STDERR_FILE_NO, "ls : path to long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            
            if (stat(buf, &st) < 0) {
                fprintf(STDERR_FILE_NO, "ls : cann't stat %s\n", buf);
            }
            if (st.type == T_DIR)
                find(buf, name);
            else if (strcmp(name, getname(buf)) == 0)
                printf("%s\n", buf);
        }
        break;
    default:
        break;
    }
    close(fd);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(STDERR_FILE_NO, "Usage: find <path> <name>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}

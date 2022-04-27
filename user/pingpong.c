#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define Pread 0
#define Pwrite 1

int main(int argv, int **argc) {
    int parent[2];
    int child[2];
    if (pipe(parent) < 0 || pipe(child) < 0) {
        fprintf(2, "pipe err.\n");
        exit(1);
    }

    int res = fork();
    if (res < 0) {
        fprintf(2, "fork error.\n");
        close(parent[Pread]);
        close(parent[Pwrite]);
        close(child[Pwrite]);
        close(child[Pread]);
        exit(1);
    } else if (res == 0) {  //child
        close(parent[Pwrite]);
        close(child[Pread]);
        char ch;
        if (read(parent[Pread], &ch, sizeof(ch)) != sizeof(char)) {
            exit(1);
        } else {
            fprintf(1, "%d: received p%cng\n", getpid(), ch);
        }
        close(parent[Pread]);
        ch = 'o';
        if (write(child[Pwrite], &ch, sizeof(ch)) != sizeof(char)) {
            fprintf(2, "child write errpr!");
            exit(1);
        }
        close(child[Pwrite]);
        exit(0);
    } else {
        close(parent[Pread]);
        close(child[Pwrite]);
        char ch = 'i';

        if (write(parent[Pwrite], &ch, sizeof(ch)) != sizeof(char)) {
            fprintf(2, "parent write error!");
            exit(1);
        }
        close(parent[Pwrite]);
        if (read(child[Pread], &ch, sizeof(ch)) != sizeof(char)) {
            fprintf(2, "parent reead error!");
            exit(1);
        } else {
            fprintf(1, "%d: received p%cng\n", getpid(), ch);
        }
        close(child[Pread]);
        exit(0);
    }
}
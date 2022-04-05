#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define STDIN_FILE_NO 0
#define STDOUT_FILE_NO 1
#define STDERR_FILE_NO 2

#define PIPE_READ 0
#define PIPE_WRITE 1

int main(int argv, int **argc) {
    int p2c[2];
    int c2p[2];
    if (pipe(p2c) < 0 || pipe(c2p) < 0) {
        fprintf(STDERR_FILE_NO, "pipe err.\n");
    }
    int res = fork();
    int number = 6;
    if (res < 0) {
        fprintf(STDERR_FILE_NO, "fork error.\n");
        exit(1);
    } else if (res == 0) {  //kid
        close(p2c[PIPE_WRITE]);
        close(c2p[PIPE_READ]);
        read(p2c[PIPE_READ], &number, sizeof(number));
        close(p2c[PIPE_READ]);
        printf("<%d>: received ping\n", getpid());
        write(c2p[PIPE_WRITE], &number, sizeof(number));
        close(c2p[PIPE_WRITE]);
    } else {
        close(p2c[PIPE_READ]);
        close(c2p[PIPE_WRITE]);
        write(p2c[PIPE_WRITE], &number, sizeof(number));
        close(p2c[PIPE_WRITE]);
        read(c2p[PIPE_READ], &number, sizeof(number));
        close(c2p[PIPE_READ]);
        printf("<%d>: received pong\n", getpid());
    }
    exit(0);
}
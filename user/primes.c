#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define STDIN_FILE_NO 0
#define STDOUT_FILE_NO 1
#define STDERR_FILE_NO 2

#define PIPE_READ 0
#define PIPE_WRITE 1

void kids(int *bp) {
    int number = 0;
    read(bp[PIPE_READ], &number, sizeof(number));
    if (number == -1) 
        exit(0);
    printf("primes %d\n", number);

    int p2c[2];
    if (pipe(p2c) < 0) {
        fprintf(STDERR_FILE_NO, "pipe error.\n");
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(STDERR_FILE_NO, "fork error.\n");
    } else if (pid == 0) {
        close(p2c[PIPE_WRITE]);
        kids(p2c);
        exit(0);
    } else {
        close(p2c[PIPE_READ]);
        int tmp = 0;
        while (tmp != -1) {
            read(bp[PIPE_READ], &tmp, sizeof(tmp));
            if (tmp % number != 0) {
                write(p2c[PIPE_WRITE], &tmp, sizeof(tmp));
            }
        }
        int end = -1;
        write(p2c[PIPE_WRITE], &end, sizeof(end));
        wait(0);
    }
    exit(0);
}

int main(int argc, char const *argv[]) {
    int p2c[2];
    if (pipe(p2c) < 0) {
        fprintf(STDERR_FILE_NO, "pipe errpr.\n");
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(STDERR_FILE_NO, "fork error.\n");
    } else if (pid == 0) {
        close(p2c[PIPE_WRITE]);
        kids(p2c);
        exit(0);
    } else {
        close(p2c[PIPE_READ]);
        for (int i = 2; i < 35; ++i) {
            write(p2c[PIPE_WRITE], &i, sizeof(i));
        }
        int end = -1;
        write(p2c[PIPE_WRITE], &end, sizeof(end));
        wait(0);
    }
    exit(0);
}

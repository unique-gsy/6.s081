#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define Pread 0
#define Pwrite 1

int read_left_first(int left[2], int* number) {
    if (read(left[Pread], number, sizeof(int)) != sizeof(int)) {
        fprintf(2, "kid read error!");
        return -1;
    }
    if (*number != -1) {
        printf("primes %d\n", *number);
        return 0;
    }
    return -1;
}

void trans_data(int left[2], int right[2], int first) {
    int number = first;
    while (number != -1) {
        if (read(left[Pread], &number, sizeof(number)) != sizeof(int)) {
            fprintf(2, "kid read error!");
            exit(1);
        }
        if (number != -1 && number % first)
            write(right[Pwrite], &number, sizeof(number));
        
    }
    write(right[Pwrite], &number, sizeof(number));
}

void kids(int *left) {
    int number = 0;
    close(left[Pwrite]);
    if (read_left_first(left, &number) == 0) {
        int right[2];
        if (pipe(right) < 0) {
            fprintf(2, "pipe error.\n");
            exit(1);
        }

        int pid = fork();
        if (pid < 0) {
            fprintf(2, "fork error.\n");
            close(right[Pwrite]);
            close(right[Pread]);
            exit(1);
        } else if (pid == 0) {
            kids(right);
            exit(0);
        } else {
            trans_data(left, right, number);
            close(left[Pread]);
            wait(0);
            exit(0);
        }
    }
    exit(1);
}

int main(int argc, char const *argv[]) {
    int right[2];
    if (pipe(right) < 0) {
        fprintf(2, "pipe errpr.\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error.\n");
        exit(1);
    } else if (pid == 0) {  //kid
        kids(right);
    } else {
        close(right[Pread]);
        for (int i = 2; i <= 35; ++i)
            write(right[Pwrite], &i, sizeof(i));
        int end = -1;
        write(right[Pwrite], &end, sizeof(end));
        close(right[Pwrite]);
        wait(0);
    }
    exit(0);
}

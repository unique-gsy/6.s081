#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define STDIN_FILE_NO 0
#define STDOUT_FILE_NO 1
#define STDERR_FILE_NO 2

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(STDERR_FILE_NO, "usage: need more parameter.\n");
    } else if (argc > 2) {
        fprintf(STDERR_FILE_NO, "usage: much parameter.\n");
    }
    int number = atoi(argv[1]);
    sleep(number);
    exit(0);
}
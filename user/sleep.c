#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sleep ticks\n");
        exit(1);
    }
    int ticks = atoi(argv[1]);
    if (ticks < 0) {
        printf("Sleep time must be non-negative\n");
        exit(1);
    }
    sleep(ticks);
    exit(0);
}

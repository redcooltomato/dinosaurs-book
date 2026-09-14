#include "pid-manager.c"
#include <stdio.h>

void debug_print() {
    printf("current pid bitset state:\n||");
    for (int sector = MIN_PID / 8; sector < MAX_PID / 8 + (MAX_PID % 8 > 0); sector++) {
        for (int i = 0; i <= 7; i++) {
            printf("%d", (pid_bitset[sector] & (1 << (7 - i))) > 0);
        }
        printf("||");
    }
    printf("\n");
}

int main(void)
{
    if (allocate_map() == -1) {
        printf("failed to allocate map for pid manager\n");
        
        return 0;
    }

    printf("usable assigned pids: %d, total allocated pids: %d\n", MAX_PID - MIN_PID + 1, MAX_PID / 8 + (MAX_PID % 8 > 0));

    debug_print();

    for (int i = MIN_PID; i < MIN_PID + 11; i++) {
        printf("allocated pid: %d\n", allocate_pid());
    }

    debug_print();

    for (int i = MIN_PID; i < MIN_PID + 3; i++) {
        release_pid(i);
        printf("freed pid: %d\n", i);
    }

    debug_print();

    for (int i = MIN_PID; i < MIN_PID + 5; i++) {
        printf("allocated pid: %d\n", allocate_pid());
    }

    debug_print();

    return 0;
}
#define MIN_PID 0
#define MAX_PID 10

char pid_bitset[MAX_PID / 8 + (MAX_PID % 8 > 0)];

int allocate_map(void)
{
    for (int i = 0; i < MAX_PID / 8 + (MAX_PID % 8 > 0); i++) {
        pid_bitset[i] = 0;
    }

    return 1;
}

int allocate_pid(void)
{
    int selected_pid = -1;

    for (int sector = MIN_PID / 8; sector < MAX_PID / 8 + (MAX_PID % 8 > 0); sector++) {
        if (pid_bitset[sector] == 0xFF)
            continue;

        for (int bit = 0; bit <= 7; bit++) {
            if ((int)(pid_bitset[sector] & (1 << (7 - bit))) == 0x0
                && sector * 8 + bit <= MAX_PID
                && sector * 8 + bit >= MIN_PID) {

                selected_pid = sector * 8 + bit;
                pid_bitset[sector] = pid_bitset[sector] | (1 << (7 - bit));

                break;
            }
        }

        if (selected_pid != -1)
            break;
    }

    return selected_pid;
}

void release_pid(int pid) {
    pid_bitset[pid / 8] = pid_bitset[pid / 8] & ~(1 << (7 - (pid % 8)));
}
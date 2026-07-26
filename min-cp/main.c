#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
    char src_path[PATH_MAX], dest_path[PATH_MAX];

    switch (argc) {
        case 1:
            printf("usage:\n%s [source] [destination]\n", argv[0]);
            return 0;
        case 2:
            printf("missing destination file path\n");
            return 0;
        case 3:
            break; // do nothing
        default:
            printf("too many arguments\n");
            return 0;
    }

    strncpy(src_path, argv[1], PATH_MAX);
    strncpy(dest_path, argv[2], PATH_MAX);

    int src = open(src_path, O_RDONLY);
    if (src == -1) {
        printf("error occured while opening source file: %d\n", errno);
        return 1;
    }
    int dest = open(dest_path, O_WRONLY);
    if (dest == -1) {
        printf("error occured while opening dest file: %d\n", errno);
        return 1;
    }

    const int RW_SIZE = (1 << 16);
    char buf[RW_SIZE];

    ssize_t read_ret, write_ret;
    do {
        read_ret = read(src, buf, RW_SIZE);
        if (read_ret == -1) break;

        write_ret = write(dest, buf, read_ret);
    } while (read_ret > 0 && read_ret == RW_SIZE && write_ret >= 0);

    if (read_ret == -1) {
        printf("error occured while reading: %d\n", errno);
        return 1;
    }
    if (write_ret == -1) {
        printf("error occured while writing: %d\n", errno);
        return 1;
    }

    printf("success!\n");

    return 0;
}
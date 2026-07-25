#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <linux/limits.h>


int main() { // ignore int argc, char *argv[]
    char src_path[PATH_MAX], dest_path[PATH_MAX];

    printf("path to the file contents of which to copy:\n");
    scanf("%s", src_path);
    printf("path to the file contents of which are to be overwritten:\n");
    scanf("%s", dest_path);

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

    const int RW_SIZE = 65536;
    int buf[RW_SIZE]; // int cuz why not

    ssize_t read_ret, write_ret;
    do {
        read_ret = read(src, buf, RW_SIZE);
        if (read_ret == -1) break;

        write_ret = write(dest, buf, read_ret);
    } while (read_ret > 0 && read_ret == RW_SIZE / sizeof(int) && write_ret >= 0);

    if (read_ret == -1) {
        printf("error occured while reading: %d\n", errno);
        return 1;
    }
    if (write_ret == -1) {
        printf("error occured while writing: %d\n", errno);
        return 1;
    }

    printf("success!\n");
}
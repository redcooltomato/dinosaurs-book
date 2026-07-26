#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>


void handle_open_errno(char *file_opened);

void handle_rw_errno(char *file_opened);

int main(int argc, char *argv[])
{
    char src_path[PATH_MAX], dest_path[PATH_MAX];

    switch (argc) {
        case 0:
            return 1; // can't really happen
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
        handle_open_errno("source");
        return 1;
    }

    int dest = open(dest_path, O_WRONLY);
    if (dest == -1) {
        handle_open_errno("destination");
        return 1;
    }

    const int RW_SIZE = (1 << 16);
    char buf[RW_SIZE];

    ssize_t read_ret, write_ret;
    do {
        read_ret = read(src, buf, RW_SIZE);
        if (read_ret == -1) break;

        write_ret = write(dest, buf, read_ret);
    } while (read_ret == RW_SIZE && read_ret > 0 && write_ret >= 0);

    if (read_ret == -1) {
        handle_rw_errno("source");
        return 1;
    }
    if (write_ret == -1) {
        handle_rw_errno("destination");
        return 1;
    }

    printf("success!\n");

    return 0;
}

void handle_open_errno(char *file_opened)
{
    switch (errno) {
        default:
            printf("error occured while opening %s file: %d\n", file_opened, errno);
            break;
        case ENOENT:
            printf("%s file doesn't exist\n", file_opened);
            break;
        case EACCES:
            printf("not enough permissions to open %s file\n", file_opened);
            break;
        case ETXTBSY:
            printf("%s is currently being executed\n", file_opened);
            break;
    }
}

void handle_rw_errno(char *file_opened)
{
    switch (errno) {
        default:
            printf("error occured while reading: %d\n", errno);
            break;
        case EINTR:
            printf("%s was interrupted\n", file_opened);
            break;
        case EIO:
            printf("io error on %s\n", file_opened);
            break;
        case EISDIR:
            printf("%s file is a directory\n", file_opened);
            break;
        case ENOSPC:
            printf("no space left to write to destination file\n"); // this one is obvious
            break;
        case EPERM:
            printf("%s file is sealed\n", file_opened);
            break;
    }
}
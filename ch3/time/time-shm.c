#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    if (argc == 1) {
        printf("call with a programm to track execution time for\ne.g. %s ls -ah\n", argv[0]);

        return 0;
    }

    const int SIZE = sizeof(struct timeval);
    const char *NAME = "PROG_EXEC_TIME_CNT";

    int fd;
    struct timeval *shm_ptr, new_time;

    fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    shm_ptr = (struct timeval *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("starting the timer...\n");

    int pid = fork();
    if (pid < 0) {
        printf("failed to fork\n");
    } else if (pid == 0) {
        gettimeofday(&*shm_ptr, NULL);

        execvp(argv[1], argv + 1);
    } else {
        wait(NULL);

        gettimeofday(&new_time, NULL);

        printf("command was executing for %Lf\n",
            new_time.tv_sec - shm_ptr->tv_sec + (new_time.tv_usec - shm_ptr->tv_usec) / (long double)1000000);
    }

    return 0;
}
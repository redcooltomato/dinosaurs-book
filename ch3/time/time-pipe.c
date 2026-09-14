#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    if (argc == 1) {
        printf("call with a programm to track execution time for\ne.g. %s ls -ah\n", argv[0]);

        return 0;
    }

    const int SIZE = sizeof(struct timeval);

    const int READ_END = 0, WRITE_END = 1;
    int fd[2];

    struct timeval start_time, end_time;

    if (pipe(fd) == -1) {
        printf("pipe failed\n");
        return 0;
    }

    printf("starting the timer...\n");

    int pid = fork();
    if (pid < 0) {
        printf("failed to fork\n");
    } else if (pid == 0) {
        close(fd[READ_END]);

        gettimeofday(&start_time, NULL);

        write(fd[WRITE_END], &start_time, SIZE);

        execvp(argv[1], argv + 1);
    } else {
        close(fd[WRITE_END]);

        wait(NULL);

        gettimeofday(&end_time, NULL);

        read(fd[READ_END], &start_time, SIZE);

        printf("command was executing for %Lf\n",
            end_time.tv_sec - start_time.tv_sec + (end_time.tv_usec - start_time.tv_usec) / (long double)1000000);
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE (size_t)128
#define MAX_ARGS MAX_LINE / 2 + 1

int main(void)
{
    char *args[MAX_ARGS];
    char input[MAX_LINE];
    char *token;
    int argn = 0;

    int pid;
    int exec_in_bg;

    printf("use exit to exit the shell\n");

    for (;;) {
        printf("qsh > ");
        fflush(stdout);

        if (fgets(input, MAX_LINE, stdin) == NULL) {
            printf("unknown error has occured while getting input\n");
            break;
        }

        argn = 0;
        exec_in_bg = 0;

        token = strtok(input, " \t\n");
        if (token != NULL && strcmp(token, "exit") == 0)
            break;

        while (token != NULL && argn < MAX_ARGS - 1) {
            if (strcmp(token, "&") == 0) {
                exec_in_bg = 1;
            } else {
                args[argn++] = token;
            }

            token = strtok(NULL, " \t\n");
        }

        args[argn] = NULL;

        pid = fork();
        if (pid < 0) {
            printf("error occured while forking\n");
            break;
        } else if (pid == 0) {
            execvp(args[0], args);
        } else {
            if (exec_in_bg == 0) {
                waitpid(pid, NULL, 0);
            }
        }
    }

    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(void)
{
    time_t start = time(NULL);
    
    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
    } else if (pid == 0) {
        printf("born. dying\n");
        // die
    } else {
        printf("gave brith. waiting to die\n");
        while (time(NULL) - start < 10) {} // die after 10 seconds
    }

    return 0;
}
// #define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include "wait_status.h"

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("Usgae: %s [exit-status]\n", argv[0]);
    }

    switch (fork()) {
        case -1:
            printf("fork() err\n");
            return -1;
        case 0: 
            printf("Child_%ld started...\n", (long)getpid());
            if (argc > 1) {
                exit(atoi(argv[1]));
            } else {
                for(;;) {
                    pause();
                }
            }
            return 0;
        default:
            int child_pid, status;
            printf("Parent pid = %ld\n", (long)getpid());
            for (;;) {
                child_pid = waitpid(-1, &status, WUNTRACED | WCONTINUED); //能发现终止进程,停止进程和恢复执行的进程
                if (-1 == child_pid) {
                    printf("waitpid() err\n");
                    return -1;
                }
                printf("waitpid() return: PID=%ld, status=0x%04X(%d,%d)\n", (long)child_pid, (unsigned int)status, status >> 8, status & 0xFF);
                PrintWaitStatus(NULL, status);
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    return 0;
                }
            }
    }
}
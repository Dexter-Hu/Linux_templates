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

#include "current_time.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usgae: %s sleep-time ...\n", argv[0]);
    }
    setbuf(stdout, NULL); //取消缓冲模式

    for (int i = 1; i < argc; i++) {
        switch(fork()) {
            case -1:
                printf("fork() err\n");
                return -1;
                break;
            case 0: //子进程
                printf("[%s]:%ld Child ready to sleep %ds\n", GetCurrentTime("%T"), (long)getpid(), atoi(argv[i]));
                int sleep_time;
                sleep_time = atoi(argv[i]);
                sleep(sleep_time);
                _exit(0);
                break;
            default:
                break;
        }
    }

    int num_dead = 0;
    int child_pid;
    for (;;) {
        child_pid = wait(NULL);
        if (-1 == child_pid) {
            if (errno == ECHILD) {
                printf("No more children...\n");
                return 0;
            } else {
                printf("wait() err\n");
                return -1;
            }
        }
        num_dead++;
        printf("[%s]: Child process_%ld dead(num_dead=%d)\n", GetCurrentTime("%T"), (long)child_pid, num_dead);
    }
}
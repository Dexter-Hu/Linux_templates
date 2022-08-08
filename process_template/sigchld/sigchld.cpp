/**
 * @file      sigchld.cpp
 * @brief     用信号处理函数获取子进程终止状态的例子
 * @author    sleep-at-11(sleep-at-11.com)
 * @version   1.0.0
 * @date      2022-08-02
 * 
 * @copyright Copyright (c) 2022
 * 
 * @par 修改日志:
 * Date         Version   Author        Description
 * 2022-08-02   1.0.0     sleep-at-11   +新建
 */
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
#include "wait_status.h"

static volatile int num_live_children = 0;

static void SigchldHanlder(int sig)
{
    int saved_errno = errno; //需要临时保存一下errno
    printf("[%s]: Caught SIGCHLD signal\n", GetCurrentTime("%T"));
    pid_t child_pid;
    int status;
    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) { //抓到已终止的进程了
        printf("[%s]: Child PID = %ld\n", GetCurrentTime("%T"), (long)child_pid);
        PrintWaitStatus(NULL, status);
        num_live_children--;
    }

    if (-1 == child_pid && errno != ECHILD) {
        printf("waitpid() err\n");
        return;
    }

    sleep(5);
    printf("[%s]: Returning\n", GetCurrentTime("%T"));
    errno = saved_errno;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usgae: %s sleep-time ...\n", argv[0]);
    }
    num_live_children = argc - 1;
    setbuf(stdout, NULL);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SigchldHanlder;
    int ret = sigaction(SIGCHLD, &sa, NULL);
    if (-1 == ret) {
        printf("sigaction() err\n");
        return -1;
    }

    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD); //先阻塞
    ret = sigprocmask(SIG_SETMASK, &block_mask, NULL);
    if (-1 == ret) {
        printf("sigprocmask() err\n");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        switch (fork()) {
            case -1:
                printf("fork() err\n");
                return -1;
            case 0:
                sleep(atoi(argv[i]));
                printf("[%s]: Child_%d(PID=%ld) exiting...\n", GetCurrentTime("%T"), i, (long)getpid());
                _exit(0);
            default:
                break;
        }
    }

    sigset_t empty_mask;
    sigemptyset(&empty_mask);
    int sig_cnt = 0;
    while (num_live_children > 0) {
        ret = sigsuspend(&empty_mask);
        if (-1 == ret && errno != EINTR) {
            printf("sigsuspend() err\n");
            return -1;
        }
        sig_cnt++;
    }
    printf("[%s]: All %d children have terminated: we caught %d times\n", GetCurrentTime("%T"), argc - 1, sig_cnt); //信号不排队,所以抓到的跟实际的子进程数会不一样
}
// #define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#include "current_time.h"

#define SYNC_SIG    SIGUSR1 

#ifdef __linux__
    #define HAVE_ON_EXIT //只有少数的unix实现才有on_exit()接口
#endif

static void handler(int sig) {}

static void AtExitFunc1(void)
{
    printf("atexit function_1 called\n");
}

static void AtExitFunc2(void)
{
    printf("atexit function_2 called\n");
}

#ifdef HAVE_ON_EXIT
    static void OnExitFunc(int status, void *arg)
    {
        printf("on_exit function called: status=%d, arg=%ld\n", status, (long)arg);
    }
#endif

static void RegisterExitFunc()
{
    int ret;
#ifdef HAVE_ON_EXIT
    ret = on_exit(OnExitFunc, (void*)10);
    if (ret != 0) {
        printf("on_exit() err\n");
        return;
    }
#endif   

    ret = atexit(AtExitFunc1);
    if (ret != 0) {
        printf("atexit() err\n");
        return;
    }

    ret = atexit(AtExitFunc2);
    if (ret != 0) {
        printf("atexit() err\n");
        return;
    }

#ifdef HAVE_ON_EXIT
    ret = on_exit(OnExitFunc, (void*)20);
    if (ret != 0) {
        printf("on_exit() err\n");
        return;
    }
#endif
}

int main()
{
    sigset_t block_mask, origin_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SYNC_SIG);
    int ret = sigprocmask(SIG_BLOCK, &block_mask, &origin_mask);
    if (-1 == ret) {
        printf("sigprocmask() err\n");
        return -1;
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags   = SA_RESTART;
    sa.sa_handler = handler;
    ret = sigaction(SYNC_SIG, &sa, NULL);
    if (-1 == ret) {
        printf("sigaction() err\n");
        return -1;
    }

    int pid;
    switch (pid = fork()) {
        case -1:
            printf("fork() err\n");
            return -1;
            break;
        case 0: //子进程
            printf("[%s]:%ld Child started - doing something\n", GetCurrentTime("%T"), (long)getpid());
            RegisterExitFunc(); 
            sleep(2);
            printf("[%s]:%ld Child about to signal parent\n", GetCurrentTime("%T"), (long)getpid());
            ret = kill(getppid(), SYNC_SIG); //等2s后给父进程发送SYNC_SIG信号
            if (-1 == ret) {
                printf("kill() err\n");
                _exit(-1);
            }
            _exit(0); //子进程实际是不会调用退出函数的
            break;
        default: //父进程
            printf("[%s]:%ld Parent wait for a signal...\n", GetCurrentTime("%T"), (long)getpid());
            RegisterExitFunc();
            sigset_t empty_mask;
            sigemptyset(&empty_mask);
            ret = sigsuspend(&empty_mask);
            if (-1 == ret && errno != EINTR) {
                printf("sigsuspend() err\n");
                return -1;
            }
            printf("[%s]:%ld Parent get a signal...\n", GetCurrentTime("%T"), (long)getpid());
            ret = sigprocmask(SIG_SETMASK, &origin_mask, NULL);
            if (-1 == ret) {
                printf("sigprocmask() err\n");
                return -1;
            }
            return 0;
            break;
    }
    return 0;
}
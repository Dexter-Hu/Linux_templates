/**
 * @file      pipe_sync.cpp
 * @brief     pipe用于同步进程,父进程等待所有子进程执行完毕后再去执行
 * @author    sleep-at-11(sleep-at-11.com)
 * @version   1.0.0
 * @date      2022-08-03
 * 
 * @copyright Copyright (c) 2022
 * 
 * @par 修改日志:
 * Date         Version   Author        Description
 * 2022-08-03   1.0.0     sleep-at-11   +新建
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

#define BUF_SIZE    10

int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s sleep-time...\n", argv[0]);
        return -1;
    }

    setbuf(stdout, NULL);
    printf("[%s]: Parent started\n", GetCurrentTime("%T"));
    
    int fd[2];
    int ret = pipe(fd); //创建一个管道
    if (-1 == ret) {
        printf("pipe() eerr\n");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        switch (fork()) {
        case -1:
            printf("fork() err\n");
            return -1;
        case 0:
            ret = close(fd[0]); //子进程关闭读端
            if (-1 == ret) {
                printf("close() err\n");
                return -1;
            }

            sleep(atoi(argv[i])); //子进程休眠一段时间

            printf("[%s]: Child_%d(PID=%ld) closing pipe\n", GetCurrentTime("%T"), i, (long)getpid());
            
            ret = close(fd[1]); //子进程再关闭写端
            if (-1 == ret) {
                printf("close err\n");
                return -1;
            }
            printf("Child exiting...\n");
            return 0;
        default:
            break;
        }
    }
    
    ret = close(fd[1]); //父进程关闭写端
    if (-1 == ret) {
        printf("close err\n");
        return -1;
    }

    int dummy;
    if (read(fd[0], &dummy, 1) != 0) { //正常情况下父进程这里会等所有子进程的写端关闭
        printf("Parent read err\n");
        return -1;
    }
    printf("[%s]: Parent exiting...\n", GetCurrentTime("%T"));
    return 0;
}
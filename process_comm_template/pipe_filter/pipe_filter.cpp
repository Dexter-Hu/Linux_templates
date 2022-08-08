/**
 * @file      pipe_filter.cpp
 * @brief     pipe一端复制为标准输出,一端复制为标准输入,形成过滤器
 * @author    sleep-at-11(sleep-at-11.com)
 * @version   1.0.0
 * @date      2022-08-04
 * 
 * @copyright Copyright (c) 2022
 * 
 * @par 修改日志:
 * Date         Version   Author        Description
 * 2022-08-04   1.0.0     sleep-at-11   +新建
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

int main(int argc, char *argv[])
{
    int fd[2];
    int ret = pipe(fd); //创建一个管道
    if (-1 == ret) {
        printf("pipe() eerr\n");
        return -1;
    }

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

            if (fd[1] != STDOUT_FILENO) {
                ret = dup2(fd[1], STDOUT_FILENO);
                if (-1 == ret) {
                    printf("Child dup2 err\n");
                    _exit(-1);
                }

                ret = close(fd[1]); //也可以关STDOUT_FILENO,一样的
                if (-1 == ret) {
                    printf("Child close err\n");
                    _exit(-1);
                }
            }

            execlp("ls", "ls", (char*)NULL); //子进程输出ls
            exit(0);
        default:
            break;
    }

    switch (fork()) { //创建第二个子进程
        case -1:
            printf("fork() err\n");
            return -1;
        case 0:
            ret = close(fd[1]); //第二个子进程关闭写端
            if (-1 == ret) {
                printf("close err\n");
                return -1;
            }

            if (fd[0] != STDIN_FILENO) {
                ret = dup2(fd[0], STDIN_FILENO);
                if (-1 == ret) {
                    printf("Child2 dup2 err\n");
                    _exit(-1);
                }

                ret = close(fd[0]); //也可以关STDIN_FILENO,一样的
                if (-1 == ret) {
                    printf("Child2 close err\n");
                    _exit(-1);
                }
            }

            execlp("wc", "wc", "-l", (char*)NULL); //这个子进程接收stdin的输入,然后自己再输出 wc -l
            exit(0);
        default:
            break;
    }
    
    ret = close(fd[0]); //父进程关闭管道两端,因为它用不到
    if (-1 == ret) {
        printf("Parent close fd0 err\n");
        return -1;
    }
    ret = close(fd[1]);
    if (-1 == ret) {
        printf("Parent close fd1 err\n");
        return -1;
    }
    
    if (-1 == wait(NULL)) {
        printf("Child1 get err\n");
        return -1;
    }

    if (-1 == wait(NULL)) {
        printf("Child2 get err\n");
        return -1;
    }
    
    return 0;
}
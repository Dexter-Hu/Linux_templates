/**
 * @file      pipe.cpp
 * @brief     pipe的简单使用
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
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s string\n", argv[0]);
        return -1;
    }

    int fd[2];
    int ret = pipe(fd); //创建一个管道
    if (-1 == ret) {
        printf("pipe() eerr\n");
        return -1;
    }

    ssize_t num_read, num_write;
    char buf[BUF_SIZE];
    switch (fork()) {
        case -1:
            printf("fork() err\n");
            return -1;
        case 0:
            ret = close(fd[1]); //子进程关闭写端
            if (-1 == ret) {
                printf("close() err\n");
                return -1;
            }

            for (;;) { //循环读
                num_read = read(fd[0], buf, BUF_SIZE); //阻塞读
                if (-1 == num_read) {
                    printf("read err\n");
                    return -1;
                } 

                if (0 == num_read) { //读到管道尾,也说明管道的写端都关闭了
                    break;
                }   
                
                num_write = write(STDOUT_FILENO, buf, num_read);
                if (num_write != num_read) {
                    printf("Child writes err\n");
                    return -1;
                }
            }

            write(STDOUT_FILENO, "\n", 1);
            ret = close(fd[0]); //子进程不读后也要关闭管道的读端
            if (-1 == ret) {
                printf("close err\n");
                return -1;
            }
            printf("Child exiting...\n");
            return 0;
        default:
            ret = close(fd[0]); //父进程关闭读端
            if (-1 == ret) {
                printf("close err\n");
                return -1;
            }

            if (write(fd[1], argv[1], strlen(argv[1])) != (ssize_t)strlen(argv[1])) {
                printf("Parent write err\n");
                return -1;
            }

            ret = close(fd[1]); //父进程写完后就关闭写端
            if (-1 == ret) {
                printf("Parent close err\n");
                return -1;
            }

            wait(NULL); //阻塞的等待子进程的结束
            printf("Parent exiting...\n");
            return 0;
    }
}
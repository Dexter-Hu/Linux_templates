/**
 * @file    epoll.cpp
 * @brief
 * @author  sleep-at-11(sleep-at-11.com)
 * @version 1.0.0
 * @date    2022-09-05
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * Date         Version     Author          Description
 * 2022-09-05   1.0.0       sleep-at-11     新建
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>

#define MAX_BUF       1000
#define MAX_EVENTS    5

int main(int argc, char *argv[])
{
    int epfd = epoll_create(argc - 1); //创建epoll实例
    if (epfd == -1) {
        printf("epoll_create() err\n");
        return -1;
    }

    for (int i = 1; i < argc; i++) { //打开相关的文件,并加入到兴趣列表中
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            printf("open() err\n");
            return -1;
        }
        printf("Opened\"%s\" on fd %d\n", argv[i], fd);

        struct epoll_event ev;
        ev.events  = EPOLLIN;
        ev.data.fd = fd; //ev.data.fd指向要关注的文件描述符
        int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        if (ret == -1) {
            printf("epoll_ctl() err\n");
            return -1;
        }
    }

    int num_open_fds = argc - 1;
    struct epoll_event evlist[MAX_EVENTS];
    char buf[MAX_BUF];
    while (num_open_fds > 0) {
        printf("About to epoll_wait...\n");
        int ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1); //一直阻塞
        if (ready == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                printf("epoll_wait() err\n");
                return -1;
            }
        }
        printf("Ready = %d\n", ready); //打印待处理的事件个数
        for (int i = 0; i < ready; i++) {
            printf("  fd=%d; events: %s%s%s\n", evlist[i].data.fd,
                    (evlist[i].events & EPOLLIN)  ? "EPOLLIN "  : "",
                    (evlist[i].events & EPOLLHUP) ? "EPOLLHUP " : "",
                    (evlist[i].events & EPOLLERR) ? "EPOLLERR " : "");

            if (evlist[i].events & EPOLLIN) { //数据可读
                int ret = read(evlist[i].data.fd, buf, MAX_BUF);
                if (ret == -1) {
                    printf("read() err\n");
                    return -1;
                }
                printf("    read %d bytes: %.*s\n", ret, ret, buf);
            } else if (evlist[i].events & (EPOLLHUP | EPOLLERR)) { //对端关闭,自己这边也关了
                printf("    closing fd %d\n", evlist[i].data.fd);
                int ret = close(evlist[i].data.fd);
                if (ret == -1){
                    printf("close() err\n");
                    return -1;
                }
                num_open_fds--;
            }
        }
    }
    printf("All files closed,exit\n");
    close(epfd);
    return 0;
}
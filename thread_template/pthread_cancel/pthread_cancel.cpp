/**
 * @file    pthread_cancel.cpp
 * @brief
 * @author  sleep-at-11(sleep-at-11.com)
 * @version 1.0.0
 * @date    2022-10-11
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
#include <pthread.h>
#include <errno.h>
#include <sys/epoll.h>

static void* ThreadFunc(void *arg)
{
    printf("Thread starting...\n");
    for (int i = 0; ; i++) {
        printf("Loop: %d\n", i);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t pid;
    int ret = pthread_create(&pid, NULL, ThreadFunc, NULL);
    if (ret != 0) {
        printf("pthread_create() err\n");
        return -1;
    }

    sleep(3);
    ret = pthread_cancel(pid); //主线程去取消之前创建的线程
    if (ret != 0) {
        printf("pthread_cancel() err\n");
        return -1;
    }

    void *result;
    ret = pthread_join(pid, &result);
    if (ret != 0) {
        printf("pthread_join() err\n");
        return -1;
    }

    if (result == PTHREAD_CANCELED) {
        printf("Thread was canceled\n");     //到这里说明成功取消
    } else {
        printf("Thread was not canceled\n"); //正常不会到这里,可能线程异常被取消了
    }
    return 0;
}
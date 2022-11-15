/**
 * @file    condition.cpp
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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef enum {
    TS_ALIVE,      //线程存活状态
    TS_TERMINATED, //线程终止
    TS_JOINED      //线程被回收
} THREAD_STATE_E;

typedef struct {
    long int index;
    THREAD_STATE_E state;
    pthread_t tid;
    int sleeptime;
} thread_t;

static int total_threads = 0;
static int num_live      = 0;
static int num_unjoined  = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

static void* ThreadFunc(void *arg)
{
    thread_t *thread = (thread_t*)arg;
    long int idx = thread->index;
    sleep(thread->sleeptime);
    printf("pthead_%ld terminating...\n", idx);

    int ret = pthread_mutex_lock(&mutex);
    if (ret != 0) {
        printf("ThreadFunc(): pthread_mutex_lock() err\n");
        return NULL;
    }
    num_unjoined++;
    thread->state = TS_TERMINATED;
    ret = pthread_mutex_unlock(&mutex);
    if (ret != 0) {
        printf("ThreadFunc(): pthread_mutex_unlock() err\n");
        return NULL;
    }

    ret = pthread_cond_signal(&cond);
    if (ret != 0) {
        printf("ThreadFunc(): pthread_cond_signal() err\n");
        return NULL;
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s sleep-seconds...\n", argv[0]);
    }

    thread_t *thread = (thread_t*)calloc(argc - 1, sizeof(thread_t));
    if (thread == NULL) {
        printf("calloc err\n");
        return -1;
    }

    for (int idx = 0; idx < argc - 1; idx++) { //根据输入参数创建对应线程
        thread[idx].index     = idx;
        thread[idx].sleeptime = atoi(argv[idx + 1]);
        thread[idx].state     = TS_ALIVE;
        int ret = pthread_create(&thread[idx].tid, NULL, ThreadFunc, (void *)&thread[idx]);
        if (ret != 0) {
            printf("main(): pthread_create() err\n");
            return -1;
        }
    }

    total_threads = argc - 1;
    num_live = total_threads;

    while (num_live > 0) {
        int ret = pthread_mutex_lock(&mutex);
        if (ret != 0) {
            printf("main(): pthread_mutex_lock() err\n");
            return -1;
        }

        while (num_unjoined == 0) {
            printf("pthread cond waiting...\n");
            ret = pthread_cond_wait(&cond, &mutex); //一开始主线程会阻塞在这里
            if (ret != 0) {
                printf("main(): pthread_cond_wait() err\n");
                return -1;
            }
        }

        printf("pthread join deal...\n");
        for (int idx = 0; idx < argc - 1; idx++) {
            if (thread[idx].state == TS_TERMINATED) {
                 ret = pthread_join(thread[idx].tid, NULL);
                 if (ret != 0) {
                    printf("main(): pthread_join() err\n");
                    return -1;
                 }
                 thread[idx].state = TS_JOINED;
                 num_live--;
                 num_unjoined--;
                 printf("Reaped thread[%d],num_live=%d\n", idx, num_live);
            }
        }

        ret = pthread_mutex_unlock(&mutex);
        if (ret != 0) {
            printf("main(): pthread_mutex_unlock() err\n");
            return -1;
        }
    }
    free(thread);
    return 0;
}
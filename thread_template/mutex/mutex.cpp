/**
 * @file    mutex.cpp
 * @brief 
 * @author  sleep-at-11(sleep-at-11.com)
 * @version 1.0.0
 * @date    2022-05-18
 * 
 * @copyright Copyright (c) 2022  
 * 
 * @par 修改日志:
 * Date         Version     Author          Description
 * 2022-05-18   1.0.0       sleep-at-11     新建
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if 0 //mutex基础用法
/**
 * @brief mutex基础用法
 */
void mutex_base()
{
    /** 
     *  1.  初始化互斥量,两种方式,静态和动态
     *  1.1 静态初始化;
     *  1.2 动态初始化,可以选择设置互斥量属性
     */
    /* 1.1 静态初始化互斥量 */
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 

    /* 1.2.1 动态初始化互斥量-带互斥量属性 */
    pthread_mutexattr_t mtx_attr; //互斥量属性
    pthread_mutex_t mtx;
    int ret = pthread_mutexattr_init(&mtx_attr); //初始化互斥量属性
    if (ret != 0) {
        printf("[E]: Mutex attr init err\n");
        return;
    }  
    ret = pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_ERRORCHECK); //设置互斥量属性
    if (ret != 0) {
        printf("[E]: Mutex attr set err\n");
        return;
    }
    ret = pthread_mutex_init(&mtx, &mtx_attr); //按照设置属性动态初始化互斥量
    if (ret != 0) {
        printf("[E]: pthread_mutex_init() err\n");
        return;
    }

    /* 1.2.2  动态初始化互斥量-默认属性 */
    pthread_mutex_t mtx;
    int ret = pthread_mutex_init(&mtx, &mtx_attr); //按照设置属性初始化互斥量
    if (ret != 0) {
        printf("[E]: pthread_mutex_init() err\n");
        return;
    }

    /**
     *  2.  互斥量的使用 
     *  2.1 互斥量加锁: 
     *      pthread_mutex_lock();      阻塞
     *      pthread_mutex_trylock();   非阻塞
     *      pthread_mutex_timedlock(); 带超时时间
     *  2.2 互斥量解锁
     *      pthread_mutex_unlock();
     */
    pthread_mutex_lock(&mtx);
    /* 临界区操佄1�7 */
    pthread_mutex_unlock(&mtx);

    /**
     *  3.  互斥量的释放
     *  3.1 静态互斥量不需要释放
     *  3.2 动态互斥量的释放
     *  3.2.1 设置了自定义属性的互斥量,还要释放属性的内存
     *  3.2.2 只要释放互斥量的内存即可
     */

    /* 3.2.1 释放属性内存,释放互斥量内存 */
    ret = pthread_mutexattr_destroy(&mtx_attr); //释放互斥量属性的内存
    if (ret != 0) {
        printf("[E]: pthread_mutexattr_destroy() err\n");
        return;
    }
    ret = pthread_mutex_destroy(&mtx); //释放动态申请的互斥量
    if (ret != 0) {
        printf("[E]: pthread_mutex_destroy() err\n");
        return;
    }

    /* 3.2.2 */
    ret = pthread_mutex_destroy(&mtx); //释放动态申请的互斥量
    if (ret != 0) {
        printf("[E]: pthread_mutex_destroy() err\n");
        return;
    }
}
#endif

// static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; //静态初始化互斥量
static pthread_mutex_t mtx; //互斥量
static long long int num = 0;

static void* ThreadWrite(void *arg)
{
    int loop = *((int*)arg);

    for (int i = 0; i < loop; ++i) {
        int ret = pthread_mutex_lock(&mtx);
        if (ret != 0) {
            printf("[E]: Mutex lock err\n");
            return NULL;
        }

        num++;

        ret = pthread_mutex_unlock(&mtx);
        if (ret != 0) {
            printf("[E]: Mutex unlock err\n");
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    printf("[N]: Usage: ./mutex 10000\n");

    int ret, loop;

    /**
     * 动态初始化互斥量的方式
     */
    pthread_mutexattr_t mtx_attr; //互斥量属性
    ret = pthread_mutexattr_init(&mtx_attr); //初始化互斥量属性
    if (ret != 0) {
        printf("[E]: Mutex attr init err\n");
        return -1;
    }
    
    ret = pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_ERRORCHECK); //设置互斥量属性
    if (ret != 0) {
        printf("[E]: Mutex attr set err\n");
        return -1;
    }

    ret = pthread_mutex_init(&mtx, &mtx_attr); //按照设置属性动态初始化互斥量
    if (ret != 0) {
        printf("[E]: pthread_mutex_init() err\n");
        return -1;
    }

    // ret = pthread_mutex_init(&mtx, NULL);      //按照默认属性动态初始化互斥量
    // if (ret != 0) {
    //     printf("[E]: pthread_mutex_init() err\n");
    //     return -1;
    // }

    loop = atoi(argv[1]); //循环次数
    if (loop <= 0) {
        return -1;
    }
    
    pthread_t t1, t2;

    ret = pthread_create(&t1, NULL, ThreadWrite, &loop);
    if (ret != 0) {
        printf("[E]: Create thread err\n");
        return -1;
    }

    ret = pthread_create(&t2, NULL, ThreadWrite, &loop);
    if (ret != 0) {
        printf("[E]: Create thread err\n");
        return -1;
    }

    ret = pthread_join(t1, NULL);
    if (ret != 0) {
        printf("[E]: Thread1 join err\n");
        return -1;
    } 

    ret = pthread_join(t2, NULL);
    if (ret != 0) {
        printf("[E]: Thread2 join err\n");
        return -1;
    } 

    ret = pthread_mutexattr_destroy(&mtx_attr); //释放互斥量属性的内存
    if (ret != 0) {
        printf("[E]: pthread_mutexattr_destroy() err\n");
        return -1;
    }

    ret = pthread_mutex_destroy(&mtx); //释放动态申请的互斥量内存
    if (ret != 0) {
        printf("[E]: pthread_mutex_destroy() err\n");
        return -1;
    }

    printf("[R]: Num=%lld\n", num);
    return 0;
}
/**
 * @file    timerfd.cpp
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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timerfd.h>

void ItimerspecFromStr(char *str, struct itimerspec *time) //输入格式1/2:1/2,前面是定时时间,后面是定时周期
{
    char *dup_str = strdup(str); //复制一份str

    char *sec_ptr = strchr(dup_str, ':'); //找到第一个:所在位置
    if (sec_ptr != NULL) {
        *sec_ptr = '\0'; //将:替换为\0
    }
    char *ns_ptr = strchr(dup_str, '/');
    if (ns_ptr != NULL) {
        *ns_ptr = '\0';
    }

    time->it_value.tv_sec = atoi(dup_str);
    time->it_value.tv_nsec = (ns_ptr != NULL) ? atoi(ns_ptr + 1) : 0;

    if (sec_ptr == NULL) { //单次定时器
        time->it_interval.tv_sec  = 0;
        time->it_interval.tv_nsec = 0;
    } else {
        ns_ptr = strchr(sec_ptr + 1, '/');
        if (ns_ptr != NULL) {
            *ns_ptr = '\0';
        }
        time->it_interval.tv_sec = atoi(sec_ptr + 1);
        time->it_interval.tv_nsec = (ns_ptr != NULL) ? atoi(ns_ptr + 1) : 0;
    }
    // printf("[%ld.%ld, %ld.%ld]\n",  time->it_value.tv_sec,  time->it_value.tv_nsec, time->it_interval.tv_sec, time->it_interval.tv_nsec);
    free(dup_str);
}


int main(int argc, char *argv[])
{
    struct itimerspec time;
    ItimerspecFromStr(argv[1], &time);
    int max_loop = atoi(argv[2]);

    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (-1 == fd) {
        printf("timerfd_create() err\n");
        return -1;
    }

    int ret = timerfd_settime(fd, 0, &time, NULL);
    if (-1 == ret) {
        printf("timerfd_settime() err\n");
        return -1;
    }

    struct timespec start;
    ret = clock_gettime(CLOCK_MONOTONIC, &start); //不可修改的实时时钟
    if (-1 == ret) {
        printf("clock_gettime() err\n");
        return -1;
    }
    printf("Time Start: %ld.%ld\n", start.tv_sec, start.tv_nsec);

    for (int total_loop = 0; total_loop < max_loop;) {
        static int num_loop;
        ssize_t read_num = read(fd, &num_loop, sizeof(long int)); //时间未到的话会阻塞在这里
        if (read_num != sizeof(long int)) {
            printf("read() err\n");
            return -1;
        }

        total_loop += num_loop;

        struct timespec now;
        ret = clock_gettime(CLOCK_MONOTONIC, &now);
        if (-1 == ret) {
            printf("clock_gettime() err\n");
            return -1;
        }
        printf("Time Now: %ld.%ld\n", now.tv_sec, now.tv_nsec);

        long long int secs  = now.tv_sec  - start.tv_sec;
        long long int nsecs = now.tv_nsec - start.tv_nsec;
        if (nsecs < 0) {
            secs--;
            nsecs += 1000000000;
        }

        /* 打印当前已消耗的时间,定时器已经到期的次数和总共到期的次数 */
        printf("%lld.%03lld: expirations read: %llu, total=%llu\n", secs, (nsecs + 500000) / 1000000, \
            (unsigned long long int)num_loop, (unsigned long long int)total_loop);
    }
    return 0;
}
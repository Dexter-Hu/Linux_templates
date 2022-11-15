/**
 * @file    rlimit.cpp
 * @brief
 * @author  sleep-at-11(sleep-at-11.com)
 * @version 1.0.0
 * @date    2022-11-14
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
#include <sys/resource.h>


int main(int argc, char *argv[])
{
    struct rlimit old_limit, new_limit;
    int ret = getrlimit(RLIMIT_STACK, &old_limit);
    if (ret != 0) {
        return -1;
    }
    printf("[%ld,%ld]->", old_limit.rlim_max, old_limit.rlim_cur);
    new_limit.rlim_max = old_limit.rlim_max;
    new_limit.rlim_cur = 10 << 20; //10M
    if (new_limit.rlim_max != RLIM_INFINITY && new_limit.rlim_cur > new_limit.rlim_max) {
        return -1;
    }
    ret = setrlimit(RLIMIT_STACK, &new_limit);
    if (ret != 0) {
        return -1;
    }
    ret = getrlimit(RLIMIT_STACK, &old_limit);
    if (ret != 0) {
        return -1;
    }
    printf("[%ld,%ld]\n", old_limit.rlim_max, old_limit.rlim_cur);
    return 0;
}
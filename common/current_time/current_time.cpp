/**
 * @file      current_time.cpp
 * @brief     获取当前时间
 * @author    sleep-at-11(sleep-at-11.com)
 * @version   1.0.0
 * @date      2022-08-01
 * 
 * @copyright Copyright (c) 2022
 * 
 * @par 修改日志:
 * Date         Version   Author        Description
 * 2022-08-01   1.0.0     sleep-at-11   +新建
 */
#include "current_time.h"

#define BUF_SIZE    512

char* GetCurrentTime(const char *format)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (NULL == tm) {
        return NULL;
    }

    static char buf[BUF_SIZE];
    size_t size = strftime(buf, BUF_SIZE, format, tm);
    buf[BUF_SIZE - 1] = '\0';
    return (0 == size) ? NULL : buf;
}
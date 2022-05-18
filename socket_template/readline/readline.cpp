/**
 * @file    readline.cpp
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

#include "readline.h"
#include <unistd.h>
#include <errno.h>

/**
 * @brief  读取一行数据保存到指定buffer,遇到换行符则结束,
 *         没遇到则最多读取n字节数据
 * @param  fd：    要读取的文件描述符              
 * @param  buffer：保存读取到的数据           
 * @param  n：     最多读取的字节数                
 * @return -1=错误; 其他=读取到的字节数
 */
ssize_t Readline(int fd, void *buffer, size_t n)
{
    if (n < 0 || NULL == buffer) {
        return -1;
    }

    char* buf = (char*)buffer;
    size_t total_read = 0; //总共读取到的字节数
    for (;;) {
        char ch;
        int num_read = read(fd, &ch, 1); //读取一个字节

        if (-1 == num_read) {     //读取错误
            if (EINTR == errno) { //如果是被打断的,继续读
                continue;
            } else {
                return -1;
            }
        } else if (0 == num_read) { //读到EOF
            if (0 == total_read) {  //空文件
                return 0;
            } else {
                break;
            }
        } else {
            if (total_read < n - 1) {
                total_read++;
                *buf = ch;
                buf++;
            } 
            if ('\n' == ch) { //没满之前读取到换行符了
                break;
            }
        }
    }

    *buf = '\0';
    return total_read;
}
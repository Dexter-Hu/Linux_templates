/**
 * @file    readline.h
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
#ifndef READLINE_H
#define READLINE_H

#include <sys/types.h>
ssize_t Readline(int fd, void *buffer, size_t n);

#endif
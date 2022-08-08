/**
 * @file      pipe_filter.cpp
 * @brief     pipe一端复制为标准输出,一端复制为标准输入,形成过滤器
 * @author    sleep-at-11(sleep-at-11.com)
 * @version   1.0.0
 * @date      2022-08-04
 * 
 * @copyright Copyright (c) 2022
 * 
 * @par 修改日志:
 * Date         Version   Author        Description
 * 2022-08-04   1.0.0     sleep-at-11   +新建
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

#define BUFFER_SIZE    50
int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];
    
    FILE *fp = popen("cat /etc/passwd", "r"); //执行命令后,会将结果输出保存在fp中
    fgets(buffer, BUFFER_SIZE, fp); //读取fp的内容到buffer里
    printf("%s\n", buffer);
    pclose(fp);

    return 0;
}
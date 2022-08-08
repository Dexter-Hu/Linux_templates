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
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "wait_status.h"

void PrintWaitStatus(const char *msg, int status)
{
    if (msg != NULL) {
        printf("%s", msg);
    }

    if (WIFEXITED(status)) { //1. 子进程正常终止
        printf("Child exited, status=%d\n", WEXITSTATUS(status)); //返回终止码
    } else if (WIFSIGNALED(status)) { //2. 子进程被信号杀死
        printf("Child killed by signal %d(%s)", WTERMSIG(status), strsignal(WTERMSIG(status))); //返回杀死进程的信号编号和信号名字
#ifdef WCOREDUMP //有的linux版本可能不支持
        if (WCOREDUMP(status)) {
            printf("(core dumped)"); //查询是否生成了core文件
        }
#endif
        printf("\n");
    } else if (WIFSTOPPED(status)) { //3. 子进程因信号而停止
        printf("Child stopped by signal %d(%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status))); //返回让子进程停止的信号编号的信号名字
#ifdef WIFCONTINUED //老版本的linux不支持该项
    } else if (WIFCONTINUED(status)) { //4. 子进程恢复执行
        printf("Child continued\n");
#endif
    } else {
        printf("Unknow status=%x\n", (unsigned int)status);
    }
}
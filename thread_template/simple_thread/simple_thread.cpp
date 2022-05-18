/**
 * @file    simple_thread.cpp
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
#include <string.h>

/**
 * @brief  线程demo函数
 * @param  arg              
 * @return void* 
 */
static void* ThreadFunc(void *arg)
{
    char *s = (char*)arg;
    printf("%s\n", s);
    return (void*)(strlen(s));
}

int main(int argc, char *argv[])
{
    pthread_t pid;
    int result;

    result = pthread_create(&pid, NULL, ThreadFunc, (void*)"Hello World"); //创建一个线程
    if (result != 0) {
        printf("pthread_create() err\n");
    } 

    printf("Message form main()\n");
    void *back_code;
    result = pthread_join(pid, &back_code); //back_code保存线程返回值
    if (result != 0) {
        printf("pthread_join() err\n");
    }        
    printf("Thread returnred %ld\n", (long int)back_code);
    return 0;
}
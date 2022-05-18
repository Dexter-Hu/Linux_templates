/**
 * @file    internet_socket_client.cpp
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
#include "internet_socket_client.h"
#include "readline.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>

#if 1 //常规流程-知道ip和端口号

int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0); //创建socket
    if (-1 == sock_fd) {
        printf("[E]: socket() err\n");
        return -1;
    }

    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.1.30.6", &client_addr.sin_addr.s_addr); //想连接的服务器IP地址
    client_addr.sin_port = htons(50001); //端口号
    
    if (-1 == connect(sock_fd, (struct sockaddr*)&client_addr, sizeof(struct sockaddr_in))) {
        printf("[E]: Connect err\n");
        return -1;
    }

    char data[] = "hello";
    if (write(sock_fd, data, strlen(data)) != (ssize_t)strlen(data)) { //写数据
        printf("[E]: Write data err\n");
        return -1;
    }
    if (write(sock_fd, "\n", 1) != 1) { //写换行符
        printf("[E]: Write '\\n' err\n");
        return -1;
    }

    char ack_data[DATA_MAX_SIZE];
    int num_read = Readline(sock_fd, ack_data, DATA_MAX_SIZE); //读一行数据
    if (-1 == num_read) {
        printf("[E]: Readline() err\n");
        return -1;
    } 
    if (0 == num_read) {
        printf("[E]: Unexpected EOF\n");
        return -1;
    }
    printf("[D]: Receive data: %s\n", ack_data);
    return 0;
}

#else //getaddrinfo()方法-知道主机名和服务名

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags     = AI_NUMERICHOST | AI_NUMERICSERV; //HOST名和端口号别去解析,就是数字 
    hints.ai_family    = AF_UNSPEC;
    hints.ai_socktype  = SOCK_STREAM; //TCP
    hints.ai_addr      = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next      = NULL;

    char client_address[] = "10.1.30.6";/*"127.0.0.53";*/
    struct addrinfo *result; //保存满足条件的地址结构
    if (getaddrinfo(/*argv[1]*/client_address, PORT_NUM, &hints, &result) != 0) {
        printf("[E]: getaddrinfo() err\n");
        return -1;
    }
    printf("localhost:%s\n", argv[1]);

    if (result == NULL) {
        printf("[E]: No valid address\n");
        return -1;
    }

    struct addrinfo *rp;
    int fd;
    printf("Clinet running...\n");
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (-1 == fd) { //socket创建失败,继续下一个
            continue;
        }
        /**
         * 到这里说明socket成功
         */
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1) {
            rp->ai_addr->sa_data[13] = '\0';
            printf("ip=%s\n", rp->ai_addr->sa_data);
            break; //connect成功
        }
        close(fd); //失败的话关闭fd
    }

    if (NULL == rp) { //说明所有的地址都无法创建socket或连接不上
        printf("[E]: Could not connect socket to any address.\n");
        freeaddrinfo(result);
        return -1;
    }

    char data[] = "hello";
    if (write(fd, data, strlen(data)) != (ssize_t)strlen(data)) { //写数据
        printf("[E]: Write data err\n");
        return -1;
    }
    if (write(fd, "\n", 1) != 1) { //写换行符
        printf("[E]: Write '\\n' err\n");
        return -1;
    }

    char ack_data[DATA_MAX_SIZE];
    int num_read = Readline(fd, ack_data, DATA_MAX_SIZE); //读一行数据
    if (-1 == num_read) {
        printf("[E]: Readline() err\n");
        return -1;
    } 
    if (0 == num_read) {
        printf("[E]: Unexpected EOF\n");
        return -1;
    }
    printf("[D]: Receive data: %s\n", ack_data);
    return 0;
}

#endif
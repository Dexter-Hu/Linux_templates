/**
 * @file    internet_socket_server.cpp
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
#include "internet_socket_server.h"
#include "readline.h"

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BACKLOG    50

#if 1

int main()
{ 
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0); //创建socket
    if (-1 == sock_fd) {
        printf("[E]: socket() err\n");
        return -1;
    }

    int socket_opt = 1;
    if (-1 == setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &socket_opt, sizeof(socket_opt))) { //设置socket属性
        printf("[E]: setsockopt() err\n");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port        = htons(50001);
    if (bind(sock_fd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) != 0) { //bind
        printf("[E]: bind() err\n");
        return -1;
    }

    if (-1 == listen(sock_fd, BACKLOG)) { //listen()
        printf("[E]: listen() err\n");
        return -1;
    }

    uint32_t seq_num; //序列号

    socklen_t addrlen = sizeof(struct sockaddr);
    struct sockaddr_in client_address; //保存客户端地址
    int accept_fd = accept(sock_fd, (struct sockaddr*)&client_address, &addrlen);
    if (-1 == accept_fd) {
        printf("[E]: accept() err\n");
        return -1;
    }
    char ip_address[INET_ADDRSTRLEN];
    printf("IP= %s\n", inet_ntop(AF_INET, &client_address.sin_addr.s_addr, ip_address, INET_ADDRSTRLEN));

    for (;;) {
        // socklen_t addrlen = sizeof(struct sockaddr);
        // struct sockaddr_in client_address; //保存客户端地址
        // int accept_fd = accept(sock_fd, (struct sockaddr*)&client_address, &addrlen);
        // if (-1 == accept_fd) {
        //     printf("[E]: accept() err\n");
        //     return -1;
        // }
        // char ip_address[INET_ADDRSTRLEN];
        // printf("IP= %s\n", inet_ntop(AF_INET, &client_address.sin_addr.s_addr, ip_address, INET_ADDRSTRLEN));
        /**
         * 到这里说明连接建立成功
         */
        char host[NI_MAXHOST];    //NI_MAXHOST就是定义的host的最长字节数
        char service[NI_MAXSERV]; //NI_MAXSERV就是定义的service的最长字节数
        int address_str_len = NI_MAXHOST + NI_MAXSERV + 10;
        char addrress_str[address_str_len];
        if (0 == getnameinfo((struct sockaddr*)&client_address, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0)) {
            snprintf(addrress_str, address_str_len, "%s:%s", host, service);
        } else {
            snprintf(addrress_str, address_str_len, "UNKONWN:UNKNOWN");
        }
        printf("[R]: Connection from %s\n", addrress_str); //打印clinet的IP:PORT信息

        char req_data[DATA_MAX_SIZE]; //保存client的数据
        int req_len = Readline(accept_fd, req_data, DATA_MAX_SIZE);
        if (req_len <= 0) { 
            close(accept_fd);
            return -1;
        }
        printf("[R]: Receive data: %s\n", req_data);
        char ack_data[] = "ok";
        if (write(accept_fd, ack_data, strlen(ack_data)) != (ssize_t)strlen(ack_data)) {
            printf("[E]: Ack err\n");
        }
        seq_num += req_len;
        printf("[D]: Seq num=%u\n", seq_num);
        
        if (-1 == close(accept_fd)) {
            printf("[E]: Close() err\n");
            return -1;
        } else {
            printf("[R]: Close socket\n");
        }
    }
    
    return 0;
}
#else

int main()
{
    /** 
     * 忽略SIGPIPE信号,防止服务器向一个对端已经被关闭的socket写入数据
     * 时收到SIGPIPE信号,导致服务器进程被关闭
     */
    if (SIG_ERR == signal(SIGPIPE, SIG_IGN)) { 
        printf("[E]: signal() err\n");
        return -1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags     = AI_PASSIVE | AI_NUMERICSERV; //找能用于bind的地址,阻止服务名的解析
    hints.ai_family    = AF_UNSPEC;   //IPv4 或 IPv6
    hints.ai_socktype  = SOCK_STREAM; //TCP
    hints.ai_addr      = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next      = NULL;

    struct addrinfo *result; //目的地址
    if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0) {
        printf("[E]: getaddrinfo() err\n");
        return -1;
    }

    if (NULL == result) {
        printf("[E]: No valid address\n");
        return -1;
    }

    struct addrinfo *rp;
    int socket_fd;
    printf("[D]: Server running...\n");
    for (rp = result; rp != NULL; rp = rp->ai_next) { //为每个返回的地址结构都尝试bind,直到有一个成功为止
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (-1 == socket_fd) { //失败则继续
            printf("[D]: socket() err\n");
            continue;
        }

        int socket_opt = 1;
        if (-1 == setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_opt, sizeof(socket_opt))) { //todo:还能增加设置接收超时和发送超时的选项
            printf("[E]: setsockopt() err\n");
            freeaddrinfo(result);
            return -1;
        }

        /**
         * 到这里说明socket建立成功,尝试bind
         */
        if (0 == bind(socket_fd, rp->ai_addr, rp->ai_addrlen)) { //bind成功
            break;
        }

        close(socket_fd); //bind失败的话,close文件描述符
    }

    if (NULL == rp) {
        printf("[E]: Could not bind socket to any address\n"); //所有的地址都无法bind socket
        freeaddrinfo(result);
        return -1;
    }

    if (-1 == listen(socket_fd, BACKLOG)) {
        printf("[E]: listen() err\n");
        return -1;
    }

    freeaddrinfo(result); //到这里获取到的addrinfo没用了,记得释放
    
    uint32_t seq_num; //序列号
    for (;;) {
        socklen_t addrlen = sizeof(struct sockaddr);
        struct sockaddr client_address; //保存客户端地址
        int accept_fd = accept(socket_fd, &client_address, &addrlen);
        if (-1 == accept_fd) {
            printf("[E]: accept() err\n");
            return -1;
        }
        printf("IP= ");
        for (int i = 0; i < 14; i++) {
            std::cout<<client_address.sa_data[i];
        }
        printf("\n");
        /**
         * 到这里说明连接建立成功
         */
        char host[NI_MAXHOST];    //NI_MAXHOST就是定义的host最长字节数
        char service[NI_MAXSERV]; //NI_MAXSERV就是定义的service最长字节数
        int address_str_len = NI_MAXHOST + NI_MAXSERV + 10;
        char addrress_str[address_str_len];
        if (0 == getnameinfo(&client_address, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0)) {
            snprintf(addrress_str, address_str_len, "%s:%s", host, service);
        } else {
            snprintf(addrress_str, address_str_len, "UNKONWN:UNKNOWN");
        }
        printf("[S]: Connection from %s\n", addrress_str); //打印clinet的IP:PORT信息

        char req_data[DATA_MAX_SIZE]; //保存client发送的数据
        int req_len = Readline(accept_fd, req_data, DATA_MAX_SIZE);
        if (req_len <= 0) { 
            close(accept_fd);
            continue;
        }
        printf("[R]: Receive data: %s\n", req_data);
        char ack_data[] = "ok";
        if (write(accept_fd, ack_data, strlen(ack_data)) != (ssize_t)strlen(ack_data)) {
            printf("[E]: Ack err\n");
        }
        seq_num += req_len;
        printf("[D]: Seq num=%u\n", seq_num);
        
        if (-1 == close(accept_fd)) {
            printf("[E]: Close() err\n");
            return -1;
        }
    }
    
    return 0;
}
#endif
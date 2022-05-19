# Linux_templates
Linux系统编程模板程序

## 1. 文件目录
```txt
-- socket_template: socket相关demo
|____ internet_socket: 网络套接字
      |____ internet_sokcet_client: 网络套接字客户端
      |____ internet_socket_server: 网络套接字服务器
|____ readline:        读取一行

-- thread_template:  线程相关demo
|____ mutex:         线程互斥量
|____ simple_thread: 简单线程

-- process_template: 进程相关demo
|____ posix_shm:     posix共享内存
      |____ posix_shm_create: 创建一个posix共享内存段
      |____ posix_shm_write:  往共享内存段中写数据
      |____ posix_shm_read:   读取共享内存段中的数据
      |____ posix_shm_unlink: 删除一个posix共享内存段
```
## 2. 更新记录
```txt
2022-05-19, sleep-at-11 v1.0.0
+ 新建process_templte文件夹,存放进程相关demo;
+ 新建posix共享内存相关demo;

2022-05-18, sleep-at-11 v1.0.0
+ 新建socket_template文件夹,存放socket相关demo;
+ 新建网络套接字客户端和服务端程序;
+ 新建readline公共函数;
+ 新建thread_template文件夹,存放线程相关demo;
+ 新建互斥量demo;
+ 新建线程的简单实用demo
```



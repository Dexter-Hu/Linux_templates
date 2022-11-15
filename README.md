# Linux_templates
Linux系统编程模板程序

## 1. 文件目录
```txt
-- common: 通用文件,生成库
|____ current_time: 获取当前时间,以指定格式打印
|____ wait_status:  解析子进程返回状态码


-- io_template:   io相关操作
|____ epoll:      epoll示例

-- socket_template: socket相关demo
|____ internet_socket: 网络套接字
      |____ internet_sokcet_client: 网络套接字客户端
      |____ internet_socket_server: 网络套接字服务器
|____ readline:        读取一行

-- system_template:   系统操作相关demo
|____ rlimit:         系统限制设置

-- thread_template:   线程相关demo
|____ mutex:          线程互斥量
|____ simple_thread:  简单线程
|____ condition:      条件变量
|____ pthread_cancel: 线程取消

-- process_template: 进程相关demo
|____ process:       进程基础
      |____ process:      进程创建,退出时调用退出处理函数
      |____ wait:         wait的基础使用方法,提供了一个等待所有子进程退出的例子
      |____ child_status: 获取子进程的终止状态
      |____ sigchld:      通过信号处理函数获取子进程终止状态的例子

-- process_comm_template:  进程间通信相关demo
|____ pipe:                pipe的简单使用
|____ pipe_sync:           pipe用于同步进程,父进程等待所有子进程执行完毕后再去执行

|____ posix_shm:     posix共享内存
      |____ posix_shm_create: 创建一个posix共享内存段
      |____ posix_shm_write:  往共享内存段中写数据
      |____ posix_shm_read:   读取共享内存段中的数据
      |____ posix_shm_unlink: 删除一个posix共享内存段

-- timer_template:   定时器相关demo
|____ timerfd:       定时器操作
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

2022-08-02, sleep-at-11 v1.0.0
+ 新建common文件夹,存放通用库;
+ 新增current_time库,用于获取当前时间,并以指定格式打印;
+ 新增wait_status库,用于解析子进程返回的状态码;
+ 新建process文件夹,存放进程创建demo;
+ 新增wait文件夹,展示了wait的基础使用方法,并提供了一个等待所有子进程退出的例子;
+ 新增child_status文件夹,提供了一个获取子进程终止状态的例子;
+ 新增sigchld文件夹,提供了一个通过信号处理函数获取子进程终止状态的例子.

2022-08-03, sleep-at-11 v1.0.0
+ 新建process_comm_template,保存进程间通信例程;
+ 新建pipe文件夹,pipe管道的简单使用;
+ 新建pipe_sync文件夹,pipe管道用于同步进程.

2022-11-15, sleep-at-11 v1.0.0
+ 增加io_template文件夹,里面是io相关操作demo;
+ 增加system_template文件夹,里面是包含系统相关操作的demo;
+ 线程demo中增加条件变量和线程取消两个demo;
+ 增加timer_template文件夹,里面包含定时器相关操作.
```



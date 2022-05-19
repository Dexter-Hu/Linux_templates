#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

static int posix_shm_write(const char *name, char *data, int len)
{
    if (NULL == name || NULL == data || len <= 0) {
        return -1;
    }

    int fd = shm_open(name, O_RDWR, 0); //打开一个已有的共享内存段
    if (-1 == fd) {
        printf("[E]: shm_open() err\n");
        return -1;
    }

    int ret = ftruncate(fd, len); //重新设置大小
    if (-1 == ret) {
        printf("[E]: ftruncate() err\n");
        return -1;
    }
    printf("[R]: Resized to %d bytes\n", len);
    char *addr = (char*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //建立映射关系
    if (MAP_FAILED == addr) {
        printf("[E]: mmap() err\n");
        return -1;
    }

    ret = close(fd); //建立好映射关系之后,原来共享内存段的文件描述符就没用了
    if (-1 == ret) {
        printf("[E]: close shm fd err\n");
        return -1;
    }

    memcpy(addr, data, len); //写数据进共享内存段
    return 0;
}

int main()
{
    char name[] = "/demo_shm";
    char data[] = "hello world";
    int len = strlen(data);
    int ret = posix_shm_write(name, data, len); 
    if (-1 == ret) {
        printf("[%s-%d] %s-%s: E-posix_shm_write() err\n", __FILE__, __LINE__, __DATE__, __TIME__);
    }
    return 0;
}
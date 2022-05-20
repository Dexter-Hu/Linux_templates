#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

static int posix_shm_create(const char *name, int flag, mode_t mode, size_t size)
{
    int fd = shm_open(name, flag, mode); //创建或打开一个共享内存对象
    if (-1 == fd) {
        printf("[%s-%d] %s-%s: E-shm_open() err\n", __FILE__, __LINE__, __DATE__, __TIME__);
        return -1;
    }

    int ret = ftruncate(fd, size); //设置共享内存的大小,注意fd要有写权限,否则设置会失败.
    if (-1 == ret) {
        perror("");
        printf("[%s-%d] %s-%s: E-ftruncate() err\n", __FILE__, __LINE__, __DATE__, __TIME__);
        close(fd);
        return -1;
    }

    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //做映射
    if (MAP_FAILED == addr) {
        printf("[%s-%d] %s-%s: E-mmap() err\n", __FILE__, __LINE__, __DATE__, __TIME__);
        close(fd);
        return -1;
    }
    return 0;
}

int main()
{
    char name[] = "/demo_shm";
    int flag    = O_RDWR | O_CREAT ;
    mode_t mode = 0766;
    size_t size = 100;
    int ret = posix_shm_create(name, flag, mode, size); //创建一个posix共享内存
    if (-1 == ret) {
        printf("[%s-%d] %s-%s: E-posix_shm_write() err\n", __FILE__, __LINE__, __DATE__, __TIME__);
    }
    return 0;
}
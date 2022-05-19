#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

static int posix_shm_read(const char *name)
{
    if (NULL == name) {
        return -1;
    }

    int fd = shm_open(name, O_RDONLY, 0); //只读方式打开一个已有的共享内存段
    if (-1 == fd) {
        printf("[E]: shm_open() err\n");
        return -1;
    }

    struct stat state;
    int ret = fstat(fd, &state); //获取文件属性
    if (-1 == ret) {
        printf("[E]: fstat() err\n");
        return -1;
    }

    char *addr = (char*)mmap(NULL, state.st_size, PROT_READ, MAP_SHARED, fd, 0); //建立映射关系
    if (MAP_FAILED == addr) {
        printf("[E]: mmap() err\n");
        return -1;
    }

    ret = close(fd); //建立好映射关系之后,原来共享内存段的文件描述符就没用了
    if (-1 == ret) {
        printf("[E]: close shm fd err\n");
        return -1;
    }

    printf("[R]: Content: %s\n", addr);
    return 0;
}

int main()
{
    char name[] = "/demo_shm";
    int ret = posix_shm_read(name); //读取共享内存的内容
    if (-1 == ret) {
        printf("[%s-%d] %s-%s: E-posix_shm_read() err\n", __FILE__, __LINE__, __DATE__, __TIME__);
    }
    return 0;
}
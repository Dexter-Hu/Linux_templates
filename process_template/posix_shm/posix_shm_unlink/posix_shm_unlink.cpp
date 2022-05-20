#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

static int posix_shm_unlink(const char *name)
{
    if (NULL == name) {
        return -1;
    }

    int ret = shm_unlink(name);
    if (-1 == ret) {
        printf("[E]: shm_unlink() err\n");
        return -1;
    }

    return 0;
}

int main()
{
    char name[] = "/demo_shm";
    int ret = posix_shm_unlink(name); //删除一个posix共享内存,这里其实还要注意还要关闭shm的文件描述符,然后也要使用munmap解除映射
    if (-1 == ret) {
        printf("[%s-%d] %s-%s: E-posix_shm_unlink() err\n", __FILE__, __LINE__, __DATE__, __TIME__);
    }
    return 0;
}
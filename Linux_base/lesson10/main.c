/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    int open(const char *pathname, int flags, mode_t mode);
        参数：
            - pathname：要创建的文件的路径
            - flags：对文件的操作权限和其他的设置
                - 必选项：O_RDONLY,  O_WRONLY, O_RDWR  这三个之间是互斥的
                - 可选项：O_CREAT 文件不存在，创建新文件
            - mode：八进制的数，表示创建出的新的文件的操作权限，比如：0775
            
            最终的权限是：mode & ~umask
            0777   ->   111 111 111(权限-rwxrwxrwx的二进制表示)
        &   0775   ->   111 111 101(权限-rwxrwxr-x的二进制表示)
        ----------------------------
                        111 111 101
        按位与&：0和任何数都为0
        umask的作用就是抹去某些权限。

        flags参数是一个int类型的数据，占4个字节，32位。
        flags 32个位，每一位就是一个标志位。
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {

    // 创建一个新的文件
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);//创建成功，返回int类型的文件描述符
    printf("文件描述符fd= %d\n",fd);
    
    //因为umask的存在，此处操作之后不是0777;而是0775
    //O_RDWR | O_CREAT(权限二进制按位置或，实现多种权限的并集，类似STM32的位操作)

    if(fd == -1) {//创建失败，返回-1
        perror("open");
    }

    // 关闭
    close(fd);
    return 0;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#if 0

int open(const char *pathname, int flags);//打开一个存在的文件
		params:pathname
		flags:The argument flags must include one of the following access modes: O_RDONLY,
       	O_WRONLY,  or O_RDWR.  These request opening the file read-only, write-only,
      	 or read/write, respectively.

int open(const char *pathname, int flags, mode_t mode);//创建一个不存在的文件
void perror(const char *s);作用：打印errno对应的错误描述

#endif

int main()
{
	int fd =open("a.txt",O_RDONLY);
	if(fd == -1){
		perror("open");
	}

	close(fd);
	return 0;
}

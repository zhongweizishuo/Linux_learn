#include <stdio.h>
#include <string.h>

int main(){
	
	char * buf;
	// char *buf = new char[128];//正确的
	strcpy(buf,"hello");//buf没有申请，是一个野内存，会报错
	return 0;
}

/*
生产core文件
	ulimited -a
	ulimited -c 1024

	// Ubuntu如果不产生core文件，是文件生产目录错误使用下面的命令
	sudo bash -c "echo core > /proc/sys/kernel/core_pattern "
	
	gdb core.c -g
	./a.out //生成core文件
	gdb a.out

	core-file core//查看错误信号代码

*/

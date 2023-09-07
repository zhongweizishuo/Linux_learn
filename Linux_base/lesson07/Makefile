.PHONY:all clean
#设置makefile的变量
src=add.c div.c main.c
target=app

#格式：
#目标：目标依赖
#	   命令

$(target):$(src)
	$(CC) $(src) -o $(target)

#使用规则匹配
%.o:%.c
	gcc -c $< -o $@
# add.o:add.c
# 	gcc -c add.c -o add.o
# div.o:div.c
# 	gcc -c div.c -o div.o
# main.o:main.c
# 	gcc -c main.c -o main.o	

clean:
	rm -f *.o
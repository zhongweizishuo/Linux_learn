#include<stdio.h>
#include"head.h"
/*
Linux下交叉编译，静态库的生成与使用:参看有道云笔记
gcc main.c -o app -I ./include/ -L ./lib/ -l calc
    -I 头文件目录
    -L 静态库文件目录
    -l 具体库文件的名称 （libcalc.a要写calc）
*/
int main(){
    add(1,15);
    div(15,2);
    return 0;
}
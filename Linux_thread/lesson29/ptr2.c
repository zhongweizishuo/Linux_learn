#include <stdio.h>
//二级指针的举例
void modifyPointer(int **ptr)
{
    // 函数的目的是将传入的二级指针所指向的一级指针修改
    int value = 20;
    *ptr = &value;
}

int main()
{
    int num = 10;
    int *ptr1 = &num;   // 一级指针
    int **ptr2 = &ptr1; // 二级指针

    printf("修改之前: *ptr1 = %d\n", *ptr1); // 对指针ptr1解引用得到10

    modifyPointer(&ptr1); // 传入二级指针，然后修改其一级指针

    printf("修改之后: *ptr1 = %d\n", *ptr1); // 对一级指针解引用，得到20

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("gdb test ---\n");
    int a, b, c;
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);

    printf("a=%d\n", a);
    printf("b=%d\n", b);
    printf("c=%d\n", c);

    for (int i = 0; i < c; i++)
    {
        printf("i=%d\n", i);
    }
    return 0;
}
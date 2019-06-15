#include <stdio.h>
enum {a, b};
int foo()
{
    if(sizeof(enum{b,a}) != sizeof(int))
        return a;
    return b; // C99 前，选择和迭代语句不建立其自身的块作用域（尽管若在语句中使用复合语句，则它拥有其通常的块作用域）
}
int main()
{
    printf("%d\n", foo());
    return 0;
}

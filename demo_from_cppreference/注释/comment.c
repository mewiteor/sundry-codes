#include <stdio.h>
int main(int argc, char *argv[])
{
    int a = 3;
    int b = 2;
    int c = a //* comment */ b
        ;
    printf("%d\n", c);
    return 0;
}

#include <cstdio>

int absoluteValue(int num) 
{
    if (num >= 0)
        return num;
    
    return num * -1;
}

int sum(int num1, int num2)
{
    return num1 + num2;
}

int main()
{
    printf("num1: %d.\n", absoluteValue(10));
    printf("num2: %d.\n", absoluteValue(-10));
    printf("num1 + num2: %d.\n", sum(absoluteValue(-10), 10));
    
    return 0;
}

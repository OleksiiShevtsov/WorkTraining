#include <cstdio>
#include <cstddef>
#include <iostream>

int main()
{
    unsigned short a = 0b10101010;
    printf("%hu\n", a);

    double an = 6.0221409e23;
    printf("Avogardo's Number: %le %lf %lg \n", an, an, an);
    
    char x = 'M';
    wchar_t y = L'Z';
    printf("Windows binaries start with %c%lc.\n", x, y);

    bool b1 = true, b2 = false;
    printf("%d %d\n", b1, b2);

    printf("1 == 1: %d\n", 1 == 1);

    std::size_t size = sizeof(int);
    printf("size: %zu\n", size);

    //--------------------------
    unsigned long array[] = { 0, 1, 2, 4, 3 };
    unsigned long max = 0;
    for(size_t i = 0; i < 5; i++){
        if(array[i] > max){
            max = array[i];
        }
    }
    printf("max: %lu\n", max);

    return 0;
}

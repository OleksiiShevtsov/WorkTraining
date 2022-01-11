#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cwchar> // for wprintf

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

    ////////////////////////////////////

    unsigned long array[] = { 0, 1, 2, 4, 3 };
    unsigned long max = 0;
    for(size_t i = 0; i < 5; i++){
        if(array[i] > max){
            max = array[i];
        }
    }
    printf("max: %lu\n", max);

    max = 0;
    for (unsigned long i : array) {
        if (i > max) {
            max = i;
        }
    }
    printf("max: %lu\n", max);

    int32_t array_2[] = { 10, 20, 30, 40, 50 };
    int32_t num = sizeof(array_2) / sizeof(int32_t);
    printf("num: %lu\n", num);

    ///////////////////////////////////

    char cText[] = "A book a house of gold.";
    char16_t cText16[] = u"\u4e66\u4e2d\u81ea\u6709\u9ec4\u91d1\u5c4b";

    printf("cText: %s \n", cText);
    //wprintf("cText16: %s \n", cText16);

    ////////////////////////////////////

    char alphabet[27];
    for(int i = 0; i < 26; i++){
        alphabet[i] = i + 97;
    }
    alphabet[26] = 0;
    printf("%s\n", alphabet);
    for(int i = 0; i < 26; i++){
        alphabet[i] = i + 65;
    }
    printf("%s\n", alphabet);

    ////////////////////////////////////

    enum class Muve{
        LEFT,
        RIGHT,
        FREE
    };

    Muve race = Muve::LEFT;
    switch (race) {
        case Muve::LEFT: printf("left");break;
        case Muve::RIGHT: printf("right");break;
        case Muve::FREE: printf("free");break;
        default: printf("error");
    }

    ///////////////////////////////////

    struct Coordinate{
        int32_t x;
        int32_t y;
    };

    Coordinate coordinate;
    coordinate.x = 10;
    printf("coordinate.x: %d\n", coordinate.x);

    /////////////////////////////////////

    union Variant{
        char c[10];
        int i;
        double d;
    };

    Variant v;
    v.i = 10;
    printf("v.i: %d\n", v.i);
    v.d = 1.1;
    printf("v.d: %d\n", v.d);
    printf("v.i: %d\n", v.i);

    ///////////////////////////////////

    class Year {
    public:
        void addYear(){
            year++;
        }
        bool setYear(int newYear) {
            if (newYear < 2019) { return false; }
            year = newYear;
            return true;
        }
        int getYear() {
            return year;
        }
    private:
        int year;
    };

    Year clock;
    if (!clock.setYear(2018)) {
        clock.setYear(2019);
    }
    clock.addYear();
    printf("clock.hour: %d\n", clock.getYear());

    /////////////////////////////////////////
   


    return 0;
}

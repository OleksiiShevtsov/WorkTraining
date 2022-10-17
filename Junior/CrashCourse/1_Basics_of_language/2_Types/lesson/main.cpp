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
        Year(int year){
            if (!setYear(year)) {
                m_year = 2019;
            }
        }
        ~Year(){
            printf("clear class of year");
        }
        void addYear(){
            m_year++;
        }
        bool setYear(int newYear) {
            if (newYear < 2019) { return false; }
            m_year = newYear;
            return true;
        }
        int getYear() {
            return m_year;
        }
    private:
        int m_year;
    };

    Year year{ 2020 };
    //if (!year.setYear(2018)) {
    //    year.setYear(2019);
    //}
    year.addYear();
    printf("clock.hour: %d\n", year.getYear());

    /////////////////////////////////////////
   
    int x1 = 0;     //0
    int x2{};       //0
    int x3 = {};    //0
    int x4;         //0 (maybe)

    int y1 = 10;
    int y2{ 10 };
    int y3 = { 10 };
    int y4( 10 );

    ////  POD  /////////////////////////////////////////

    struct PodStruct{
        uint32_t ui;
        char c[256];
        bool b;
    };

    PodStruct initPod1{};                   // all fields are equal to 0
    PodStruct initPod2 = {};                // all fields are equal to 0
    PodStruct initPod3{ 10, "text" };       // b = 0;
    PodStruct initPod4{ 10, "text", true }; // all fields init

    ////  POD for array  /////////////////////////////////////////

    int array1[]{ 1, 2 };   //size of array 2: 1, 2
    int array2[ 4 ];        //size of array 4: 0, 0, 0, 0
    int array3[ 4 ]{ 1, 2 };//size of array 2: 1, 2, 0, 0
    int array4[ 4 ];        //size of array 4: no init.

    ////  Full-featured classes  ////////////////////////////////////

    struct Taxonomist{
        Taxonomist(){
            printf("1 const .(no argument)\n");
        }
        Taxonomist(char c){
            printf("2 const .char: %c\n", c);
        }
        Taxonomist(int i){
            printf("3 const .int: %d\n", i);
        }
        Taxonomist(float f){
            printf("4 const .float: %f\n", f);
        }
    };

    Taxonomist taxonomist1;
    Taxonomist taxonomist2{ 'a' };
    Taxonomist taxonomist3{ 100 };
    Taxonomist taxonomist4{ 10.5f };
    Taxonomist taxonomist5( 'b' );
    Taxonomist taxonomist6 = { 'c' };
    Taxonomist taxonomist7{};
    Taxonomist taxonomist8();

    //////////////////////////////////////////////////////////////////

    float f1{ 1 };
    float f2{ 2 };
    int narrowed_result( f1/f2 );
    //int res{ f1/f2 }; //error

    ////////////////////////////////////////////////////////////////////

    struct Ork{
        char name[256] = "Murgul";
        int damage = 100;
    };

    return 0;
}

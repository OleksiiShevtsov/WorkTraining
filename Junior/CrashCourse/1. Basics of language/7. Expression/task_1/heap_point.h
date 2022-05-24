#pragma once

#include <iostream>
#include <cstddef>
#include <cstdio>
#include <new>

struct Point{
    Point() : x{}, y{}, z{} { printf( "cons: %p\n", this ); }
    ~Point(){ printf( "dest: %p\n", this ); }
    double x, y, z;
};

void heapPointCheck(){
    const auto pointSize = sizeof( Point );
    std::byte data[ 3 * pointSize ];
    printf( "data starts at %p.\n", data );

    //placement operator
    auto point1 = new( &data[ 0 * pointSize ] ) Point{};
    auto point2 = new( &data[ 1 * pointSize ] ) Point{};
    auto point3 = new( data + ( 2 * pointSize ) ) Point{};

    std::cout << typeid(point1).name() << std::endl;

    point1->~Point();
    point2->~Point();
    point3->~Point();
}

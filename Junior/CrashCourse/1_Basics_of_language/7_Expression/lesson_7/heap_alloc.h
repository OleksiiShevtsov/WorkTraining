#pragma once

#include <iostream>
#include <cstddef>
#include <new>

struct Bucket{
    const static size_t dataSize{ 4096 };
    std::byte data[ dataSize ];
};

struct Heap{

    void* allocate( size_t bytes ){
        if( bytes > Bucket::dataSize ){ throw std::bad_alloc{}; }

        for( size_t i{}; i < n_heap_buckets; i++){
            if( !bucket_used[ i ] ){
                bucket_used[ i ] = true;
                return buckets[ i ].data;
            }
        }
        throw std::bad_alloc{};
    }

    void free( void* p ){
        for( size_t i{}; i < n_heap_buckets; i++){
            if( buckets[ i ].data == p ){
                bucket_used[ i ] = false;
                return;
            }
        }
    }

    static const size_t n_heap_buckets{ 10 };
    Bucket buckets[ n_heap_buckets ]{};
    bool bucket_used[ n_heap_buckets ]{};
};

//create heap
Heap heap;

void* operator new( size_t n_bytes ){
    std::cout << n_bytes << std::endl;
    return heap.allocate( n_bytes );
}

void operator delete( void* p ){
    return heap.free( p );
}

void heapAllocCheck(){

    printf( "Bucket: %p\n", heap.buckets );
    auto myClass_1 = new unsigned int{ 100 };
    auto myClass_2 = new unsigned int{ 200 };
    printf( "myClass: %p, 0x%x\n", myClass_1, *myClass_1 );
    printf( "myClass: %p, 0x%x\n", myClass_2, *myClass_2 );
    delete myClass_1;
    delete myClass_2;

    try {
        while (true) {
            int* i_p = new int[ 1024 ];
        }
    } catch ( const std::bad_alloc& ) {
        printf( "bad alloc!\n" );
    }
}

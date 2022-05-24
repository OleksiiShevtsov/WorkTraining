#pragma once

#include <iostream>
#include <cstddef>
#include <new>

struct Bucket{
    const static size_t dataSize{ 4096 };
    std::byte data[ dataSize ];
};

struct LargeBucket{
    const static size_t dataSizeLarge{ 1048576 };
    std::byte data[ dataSizeLarge ];
};

struct Heap{

    void* allocate( size_t bytes ){
        if( bytes > LargeBucket::dataSizeLarge ){ throw std::bad_alloc{}; }

        if( bytes <= Bucket::dataSize ){
            printf( "Bucket allocate\n" );
            for( size_t i{}; i < nHeapBuckets; i++){
                if( !bucketUsed[ i ] ){
                    bucketUsed[ i ] = true;
                    return buckets[ i ].data;
                }
            }
        } else {
            printf( "Large bucket allocate\n" );
            for( size_t i{}; i < nHeapLargeBucket; i++){
                if( !largeBucketUsed[ i ] ){
                    largeBucketUsed[ i ] = true;
                    return largeBucket[ i ].data;
                }
            }
        }
        throw std::bad_alloc{};
    }

    void free( void* p ){
        for( size_t i{}; i < nHeapBuckets; i++){
            if( buckets[ i ].data == p ){
                printf( "free bucket\n" );
                bucketUsed[ i ] = false;
                return;
            }
        }
        for( size_t i{}; i < nHeapLargeBucket; i++){
            if( largeBucket[ i ].data == p ){
                printf( "free large bucket\n" );
                largeBucketUsed[ i ] = false;
                return;
            }
        }
    }

    static const size_t nHeapBuckets{ 10 };
    static const size_t nHeapLargeBucket{ 10 };

    Bucket buckets[ nHeapBuckets ];
    LargeBucket largeBucket[ nHeapLargeBucket ];

    bool bucketUsed[ nHeapBuckets ];
    bool largeBucketUsed[ nHeapLargeBucket ];
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

    try {

        int* i_p_1 = new int[ 1024 ];
        int* i_p_2 = new int[ 10240 ];

        delete i_p_1;
        delete i_p_2;

    } catch ( const std::bad_alloc& ) {
        printf( "bad alloc!\n" );
    }
}

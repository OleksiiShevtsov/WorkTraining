#pragma once

#include <cstdio>

constexpr char posA{ 65 }, posZ{ 90 }, posa{ 97 }, posz{ 122 };
constexpr bool AZWithin( char c ){ return posA <= c && posZ >= c; };
constexpr bool azWithin( char c ){ return posa <= c && posz >= c; };

struct AlphaHistogram{
    void insert( const char* s ){
        size_t index{};
        while( const auto c = s[ index ] ){
            if( AZWithin( c ) ) { counts[ c - posA ]++; }
            else if( azWithin( c ) ){ counts[ c - posa ]++; }
            index++;
        }
    }
    void print() const {
        for( auto index{ posA }; index <= posZ; index++ ){
            printf( "%c: ", index );
            auto nAsterisks = counts[ index - posA ];
            while( nAsterisks-- ) { printf( "*" ); }
            printf("\n");
        }
    }
private:
    size_t counts[ 26 ]{};
};

#pragma once

template< class Iter >
int sumInt( Iter itr1, Iter itr2 )
{
    int sum{};
    for( auto it = itr1; it != itr2; it++ ){
        sum = sum + *it;
    }
    return sum;
}

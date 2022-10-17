#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cwchar>

#include "bank.h"

int main()
{

    Bank<IAccount> b{ new CheckingAccaunt{ 1 } };

    printf( "amount: %d\n", b.performAnOperation()->getAmount() );

    return 0;
}

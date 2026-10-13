#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cwchar>

#include "bank.h"

int main()
{

    Bank<IAccount> bank{ new CheckingAccaunt{ 1 } };

    printf( "amount: %d\n", bank.performAnOperation()->getAmount() );

    return 0;
}

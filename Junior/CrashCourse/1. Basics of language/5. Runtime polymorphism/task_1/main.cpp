#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cwchar>

#include "bank.h"

int main()
{

    ConsoleLogger consoleLogger;
    FileLogger fileLogger;

    Bank bank( &consoleLogger );
    bank.makeTransfer(1000, 2000, 49.95);

    bank.setLogger( &fileLogger );
    bank.makeTransfer(1000, 2000, 49.95);

    return 0;
}

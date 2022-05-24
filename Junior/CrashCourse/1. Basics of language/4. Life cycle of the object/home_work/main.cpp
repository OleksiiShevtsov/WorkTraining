#include <cstdio>
#include <string.h>
#include "windows.h"

#include "timer.h"

int main()
{
    Timer timer( "time2" );

    Timer timerMove( "timerMove" );
    timerMove = std::move( timer );

    time_t myTime = 0;
    while(true){
        if(myTime != time( NULL ) - timerMove.getTimestamp()){
            myTime = time( NULL ) - timerMove.getTimestamp();
            std::cout << myTime << std::endl;
        }

        if(myTime == 2){ break; }
    }

    timer.printTimer();

    return 0;
}

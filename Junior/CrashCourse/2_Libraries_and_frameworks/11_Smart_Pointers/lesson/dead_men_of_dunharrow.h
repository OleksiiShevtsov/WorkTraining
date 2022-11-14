#pragma once 

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <memory>

struct DeadMenOfDunharrow
{
    DeadMenOfDunharrow( const char* m = "" ) : message{ m }
    {
		oathsToFulfill++;
	}
    ~DeadMenOfDunharrow()
    {
		oathsToFulfill--;
	}
	const char* message;
	static int oathsToFulfill;
};

int DeadMenOfDunharrow::oathsToFulfill{};

using ScopedOathbreakers = boost::scoped_ptr< DeadMenOfDunharrow >;
using SharedOathbreakers = std::shared_ptr< DeadMenOfDunharrow >;

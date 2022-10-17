#pragma once 

#include <boost/smart_ptr/scoped_ptr.hpp>

struct DeadMenOfDunharrow {
	DeadMenOfDunharrow( const char* m = "" ) : message{ m } {
		oathsToFulfill++;
	}
	~DeadMenOfDunharrow() {
		oathsToFulfill--;
	}
	const char* message;
	static int oathsToFulfill;
};

int DeadMenOfDunharrow::oathsToFulfill{};

using ScopedOathbreakers = boost::scoped_ptr< DeadMenOfDunharrow >;
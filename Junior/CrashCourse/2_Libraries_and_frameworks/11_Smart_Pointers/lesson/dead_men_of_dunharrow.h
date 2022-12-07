#pragma once 

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
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
using IntrusivPtr = boost::intrusive_ptr< DeadMenOfDunharrow >;

size_t refCount{};

// need to reload intrusive_ptr_add_ref 
// and intrusive_ptr_release functions
void intrusive_ptr_add_ref( DeadMenOfDunharrow* d )
{
	refCount++;
}

void intrusive_ptr_release( DeadMenOfDunharrow* d )
{
	refCount--;
	if ( refCount == 0 ){ delete d; }
}
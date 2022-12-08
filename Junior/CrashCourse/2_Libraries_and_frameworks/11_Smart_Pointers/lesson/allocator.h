#pragma once 

#include <new>

static size_t nAllocated, nDeallocated;

// need to model all exposed methodsand 
// class members with exact names
template < typename T >
struct JAllocator
{
	using value_type = T;
	
	JAllocator() noexcept{}
	
	template < typename U >
	JAllocator( const JAllocator< U >& ) noexcept {}

	T* allocate( size_t n )
	{
		auto p = operator new ( sizeof(T) * n );
		++nAllocated;
		return static_cast< T* >( p );
	}

	void deallocate( T* p, size_t n )
	{
		operator delete( p );
		++nDeallocated;
	}
};

// also need to reload equals and unequals
template < typename T1, typename T2 >
bool operator==(const JAllocator< T1 >&, const JAllocator< T2 >&)
{
	return true;
}

template < typename T1, typename T2 >
bool operator!=(const JAllocator< T1 >&, const JAllocator< T2 >&)
{
	return false;
}
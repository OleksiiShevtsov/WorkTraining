#pragma once

#include <chrono>

struct JStopwatch
{
	JStopwatch( std::chrono::nanoseconds& result ) : 
		mResult{ result },
		mStart{ std::chrono::high_resolution_clock::now() }{}

	~JStopwatch()
	{
		mResult = std::chrono::high_resolution_clock::now() - mStart;
	}

private:
	std::chrono::nanoseconds& mResult;
	const std::chrono::time_point< std::chrono::high_resolution_clock > mStart;
};
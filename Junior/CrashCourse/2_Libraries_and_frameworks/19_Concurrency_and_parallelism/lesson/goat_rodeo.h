#pragma once

#include <future>
#include <iostream>
#include <mutex>
#include <atomic>
#include <condition_variable>

const size_t iter{ 10 };

// mutex
void goatRodeo()
{
	int tinCansAvailable{};
	std::mutex tinCanMutex;

	auto eatCans = std::async( std::launch::async, [ & ] {
			for( size_t i{}; i < iter; i++ )
			{
				// usage lock_guard
				std::cout << " 1) eatCans -- " << std::endl;
				std::lock_guard< std::mutex > guard{ tinCanMutex };
				tinCansAvailable--;
			}
		} );
	auto depositCans = std::async( std::launch::async, [ & ] {
			for( size_t i{}; i < iter; i++ )
			{
				// usage lock and unlock
				std::cout << " 1) depositCans ++ " << std::endl;
				tinCanMutex.lock();
				tinCansAvailable++;
				tinCanMutex.unlock();
			}
		} );

	eatCans.get();
	depositCans.get();
	std::cout << "Tin cans: " << tinCansAvailable << "\n";
}

// atomic
void goatRodeo2()
{
	std::atomic_int tinCansAvailable{};

    auto eatCans = std::async(std::launch::async, [ & ] {
			for( size_t i{}; i < iter; i++ )
			{
				std::cout << " 2) eatCans -- " << std::endl;
				tinCansAvailable--;
			}
		} );
    auto depositCans = std::async( std::launch::async, [ & ] {
			for( size_t i{}; i < iter; i++ )
			{
				std::cout << " 2) depositCans ++ " << std::endl;
				tinCansAvailable++;
			}
		} );

	eatCans.get();
	depositCans.get();
	std::cout << "Tin cans: " << tinCansAvailable << "\n";
}

// variable condition
void goatRodeo3()
{
    std::mutex m;
	std::condition_variable cv;
	int tinCansAvailable{};

	auto eatCans = std::async( std::launch::async, [ & ] {
			std::unique_lock< std::mutex > lock{ m };
			cv.wait( lock, [ & ] { return tinCansAvailable == 10; });
			for (size_t i{}; i < iter; i++)
			{
				std::cout << " 3) eatCans -- " << std::endl;
				tinCansAvailable--;
			}
		} );
	auto depositCans = std::async( std::launch::async, [ & ] {
			std::scoped_lock< std::mutex > lock{ m };
			for(size_t i{}; i < iter; i++)
			{
				std::cout << " 3) depositCans ++ " << std::endl;
				tinCansAvailable++;
			}
			cv.notify_all();
		} );

	eatCans.get();
	depositCans.get();
	std::cout << "Tin cans: " << tinCansAvailable << "\n";
}

void goatRodeoCheck()
{
	goatRodeo();
	goatRodeo2();
	goatRodeo3(); 
}

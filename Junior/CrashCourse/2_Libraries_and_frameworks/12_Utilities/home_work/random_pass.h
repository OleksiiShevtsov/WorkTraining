#pragma once

#include <random>

std::mt19937_64 mtEngine{ 56756 };
std::uniform_int_distribution< int > intDist{ 0, 'z' };
const size_t passwordLength = 2;

char* randomPass() 
{	
	char newPass[ passwordLength ];

	for (size_t i{}; i < passwordLength - 1; i++)
	{
		newPass[ i ] = (char)intDist(mtEngine);
	}
	newPass[ passwordLength - 1 ] = '\0';
	return newPass;
}

void randomPassCheck()
{
	std::string pass(randomPass());

	std::cout << pass;
}

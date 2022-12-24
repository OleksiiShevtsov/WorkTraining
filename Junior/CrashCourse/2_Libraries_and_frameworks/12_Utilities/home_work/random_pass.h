#pragma once

#include <random>

std::mt19937_64 mtEngine{ 57878 };
std::uniform_int_distribution< int > intDistBool{ 0, 1 };
std::uniform_int_distribution< int > intDistNum{ '0', '9' };
std::uniform_int_distribution< int > intDistChar{ 'A', 'z' };

const size_t passwordLength = 16;

std::string randomPass() 
{	
	std::string newPass;

	for (size_t i{}; i < passwordLength; i++)
	{
		if (intDistBool(mtEngine)) 
		{
			newPass.push_back( ( char )intDistNum( mtEngine ) );
		}
		else 
		{
			newPass.push_back( ( char )intDistChar( mtEngine ) );
		}
	}
	return newPass;
}

void randomPassCheck()
{
	std::cout << randomPass() << std::endl;
}

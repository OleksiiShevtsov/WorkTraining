#include <iostream>
#include <string>

int main()
{
	int number_of_words = 0;
	std::string previous = "w"; // Не слово 
	std::string current;
	while (std::cin >> current) {
		++number_of_words; // Увеличива ем счетчик слов 
		if (previous == current) {
			std::cout << "repetition of a word: " << current
				<< " after " << number_of_words
				<< " words." << std::endl;
			previous = current;
		}
	}
}

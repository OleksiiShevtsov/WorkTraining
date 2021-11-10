#include <iostream>
#include <string>

int main()
{
	std::string prev = "w";

	std::string curr;
	while (std::cin >> curr) {
		if (prev == curr) {
			std::cout << "repit word: " << curr << std::endl;
			prev = curr;
			break;
		}
	}
}

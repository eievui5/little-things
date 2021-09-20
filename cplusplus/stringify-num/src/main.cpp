#include <iostream>
#include <string>

#include "stringify-num.h"

int main(int argc, char* argv[]) {
	int input_number;
	std::string input_string;
	if (argc > 1) {
		input_string = argv[1];
		input_number = std::stoi(argv[1]);
	} else {
		std::cout << "Enter a number: ";
		std::cin >> input_string;
		input_number = std::stoi(input_string);
	}
	std::cout << "Number " << input_string << " is " << stringify_number(input_number) << '\n';
}

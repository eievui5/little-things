#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

std::array<std::string, 10> number_digits = {
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
};

std::array<std::string, 8> number_prefixes = {
    "twen", "thir", "four", "fif", "six", "seven", "eigh", "nine"
};

// All possible suffixes for a signed 64-bit integer.
std::array<std::string, 6> hundred_suffixes = {
    "thousand", "million", "billion", "trillion", "quadrillion", "quintillion"
};

std::string stringify_hundred(uint16_t number, bool insert_and) {
	std::string result;

	// Specially handle numbers under 10 (specifically for zero).
	if (number < 10) {
		return number_digits[number];
	}
	
	// Add hundreds
	if (number >= 100) {
		result = number_digits[number / 100 % 10] + " hundred";
		// If there are no remaining digits, exit early.
		if (number % 100 == 0) {
			return result;
		} else {
			result += " ";
			result += insert_and ? "and " : "";
		}
	}
	// Check for the teen range and handle it specially.
	if (number % 100 < 20 and number % 100 > 9) {
		switch (number % 10) {
		case 0: result + "ten";
		case 1: result + "eleven";
		case 2: result + "twelve";
		default:
			return result + number_prefixes[number % 10 - 2] + "teen";
		}
	} else if (number % 100 >= 20) {
		result += number_prefixes[number / 10 % 10 - 2] + "ty";
		if (number % 10 == 0) {
			return result;
		} else {
			result += " ";
		}
	}
	return result + number_digits[number % 10];
}

std::string stringify_number(int64_t number, bool insert_and = false) {
	bool negative = number < 0;
	number = std::abs(number);
	std::string result = stringify_hundred(number % 1000, insert_and);

	int64_t factor = 1000;
	uint8_t factor_index = 0;
	while (number >= factor) {
		if (number / factor % 1000 != 0) {
			result = stringify_hundred(number / factor % 1000, insert_and) + \
				" " + hundred_suffixes[factor_index] + ((number % factor) ? (" " + result) : "");
		}
		factor *= 1000;
		factor_index += 1;
	}

	if (negative) {
		return "negative " + result;
	}
	return result;
}

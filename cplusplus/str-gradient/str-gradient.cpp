#include <cstdint>
#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Create a source string.
    std::string src_string;
    // Check for command-line arguments.
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            src_string += argv[i];
            src_string += ' ';
        }
    } else {
        // Otherwise, just ask the user for an input string.
        std::cout << "Enter a String: ";
        std::string new_input;
        while(std::cin >> new_input) {
            src_string += new_input + ' ';
        }
    }

    // Display string as a gradient/wave.
    std::string print_string = src_string;
    while (1) {
        while (print_string.length() > 1) {
            print_string.resize(print_string.length() - 1, ' ');
            std::cout << print_string << '\n';
            usleep(50000);
        }
        while (print_string.length() < src_string.length()) {
            int prev_len = print_string.length();
            print_string = src_string;
            print_string.resize(prev_len + 1, ' ');
            std::cout << print_string << '\n';
            usleep(50000);
        }
    }
}

#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

namespace matrix {

    static std::vector<std::string> line_lookup;
    static std::vector<int> line;

    inline void set_width(int width = 80) {
        line.resize(width, 0);
    }

    void set_line() {
        static const std::string DEFAULT_LOOKUP[] = {
            ".", ",", "-", "=", "#", "░", "▒", "▓", "█"};
        for (int i = 0; i < 9; i++) {
            line_lookup.push_back(DEFAULT_LOOKUP[i]);
        }
    }

    void effect() {
        std::string result;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == 0) {
                if (std::rand() % 20 == 0) {
                    line[i] = line_lookup.size() / 2 + std::rand() % (line_lookup.size() / 2) + 1;
                }
                result += ' ';
            } else {
                result += line_lookup[line[i]-- - 1];
            }
        }
        std::cout << "\033[92;49m" << result << '\n';
    }
}

static struct option const longopts[] = {
    { "direct",       required_argument, NULL, 'd' },
	{ "file",         required_argument, NULL, 'f' },
	{ "lines",        required_argument, NULL, 'l' },
	{ "width",        required_argument, NULL, 'w' },
	{ NULL,           no_argument,       NULL, 0   }
};

int main(int argc, char* argv[]) {
    int lines = 100;
    int option_char;
    int width = 80;

    matrix::set_line();

    while ((option_char = getopt_long_only(argc, argv, "f:l:w:", longopts, NULL)) != -1) {
        char *endptr;
		int value;

        switch (option_char) {
        case 'd': {
            matrix::line_lookup.clear();
            while (*optarg != 0) {
                std::string nextc(1, *optarg++);
                matrix::line_lookup.push_back(nextc);
            }
            matrix::line_lookup.push_back(" ");
            break;
        }
        case 'f': {
            std::ifstream input_file;

            input_file.open(optarg);
            if (input_file.is_open()) {
                matrix::line_lookup.clear();

                while (not input_file.eof()) {
                    std::string next_line;

                    input_file >> next_line;
                    matrix::line_lookup.push_back(next_line);
                }
            } else {
                std::cerr << "Invalid argument to '-f'!\n";
            }
            break;
        }
        case 'l': {
            value = strtoul(optarg, &endptr, 0);

			if (optarg[0] == '\0' || *endptr != '\0') {
				std::cerr << "Invalid argument for option 'l'";
			} else {
			    lines = value;
            }
            break;
        }
        case 'w': {
            value = strtoul(optarg, &endptr, 0);

			if (optarg[0] == '\0' || *endptr != '\0') {
				std::cerr << "Invalid argument for option 'w'";
			} else {
			    width = value;
            }
            break;
        }
        }
    }

    matrix::set_width(width);
    for (int i = 0; i < lines; i++) {
        matrix::effect();
        usleep(50000);
    }
}

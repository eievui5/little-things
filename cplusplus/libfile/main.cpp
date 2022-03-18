#include "file.hpp"

int main() {
    // The standard i/o streams can be accessed through three wrapper objects.
    file::stdout.puts("Hello, world!\n");

    // You can do simple output files.
    file::outfile of = "text.txt";
    of.puts("Hello, files!\n");

    // and simple input files!
    file::infile libfile = "file.hpp";
    file::stdout.puts(libfile.getall());

    // All files are automatically closed upon leaving scope :)
}

#pragma once

#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

namespace file {

// A generic file class.
class file {
public:
    std::FILE * c_file = nullptr;

    void close() {
        std::fclose(c_file);
        c_file = nullptr;
    }

    long tell() { return std::ftell(c_file); }
    void seekto(long i) { std::fseek(c_file, i, SEEK_SET); }
    void seekby(long i) { std::fseek(c_file, i, SEEK_CUR); }
    void seekend(long i) { std::fseek(c_file, i, SEEK_END); }

    // Returns true if the file is considered open; that is, if the file pointer
    // is not nullptr.
    bool is_open() { return c_file != nullptr; }
};

// A binary input file. The file is opened in "read" and "binary" mode, and
// can be used to read a sequence of bytes.
class binfile : public file {
public:
    // Open the file at `path` for reading. Returns true if the file was
    // successfully opened.
    bool open(const char * path) {
        c_file = std::fopen(path, "rb");
        return c_file != nullptr;
    }
    bool open(std::filesystem::path& path) { return open(path.c_str()); }
    bool open(std::string& path) { return open(path.c_str()); }

    // Checks if any functions read an EOF byte. This does not tell you if the
    // next byte is EOF. For that, use `peek()`.
    bool eof() { return std::feof(c_file); }
    // Return the next byte.
    int getc() { return std::fgetc(c_file); }

    // Return the next byte, without consuming it.
    int peek() {
        int next = std::fgetc(c_file);
        std::ungetc(next, c_file);
        return next;
    }

    // Read `some` bytes, writing them into `buffer`. `buffer` must be at least
    // `some` bytes long. Keep in mind that this should not be used for reading
    // strings, as it will not append a null byte to the string. Returns true
    // if all bytes were read without reaching EOF.
    bool read(char * buffer, size_t some) {
        for (; some; some--) {
            int next = getc();
            if (next == EOF) return false;
            *buffer++ = (char) next;
        }
        return true;
    }

    // Read `some` bytes, returning them as a vector of bytes.
    std::vector<char> read(size_t some) {
        std::vector<char> result;
        for (; some; some--) {
            int next = getc();
            if (next == EOF) break;
            result.push_back((char) next);
        }
        return result;
    }

    // Read the remaining bytes of the file into a vector of bytes.
    std::vector<char> readall() {
        std::vector<char> result;
        while (1) {
            int next = getc();
            if (next == EOF) break;
            result.push_back((char) next);
        }
        return result;
    }

    binfile() {}
    binfile(std::filesystem::path& path) { open(path); }
    binfile(std::string& path) { open(path); }
    binfile(const char * path) { open(path); }
    binfile(FILE * f) { c_file = f; }
    ~binfile() {if (is_open()) close();}
};

// A text input file. The file is opened in "read" mode, and has additional
// capabilities meant for files containing text.
class infile : public binfile {
public:
    // Open the file at `path` for reading. Returns true if the file was
    // successfully opened.
    bool open(const char * path) {
        c_file = std::fopen(path, "r");
        return c_file != nullptr;
    }
    bool open(std::filesystem::path& path) { return open(path.c_str()); }
    bool open(std::string& path) { return open(path.c_str()); }

    // Read `some` bytes, returning them as a string.
    std::string getsome(size_t some) {
        std::string result;
        result.reserve(some);
        for (; some; some--) {
            int next = getc();
            if (next == EOF) break;
            result += (char) next;
        }
        return result;
    }

    // Read until encountering one of the characters in `delims`. The delimiter
    // is not included in the resulting string.
    std::string getdelim(std::string& delims) {
        std::string result;
        while (1) {
            // The eof() check above allows us to safely narrow this down to
            // char.
            int next = getc();
            if (next == EOF) break;
            for (char i : delims) if (next == i) break;
            result += (char) next;
        }
        return result;
    }

    std::string getdelim(const char * delims) {
        std::string result;
        while (1) {
            // The eof() check above allows us to safely narrow this down to
            // char.
            int next = getc();
            if (next == EOF) break;
            for (const char * i = delims; *i; i++) if (next == *i) break;
            result += (char) next;
        }
        return result;
    }

    std::string getdelim(char delim) {
        std::string result;
        while (1) {
            int next = getc();
            if (next == EOF) break;
            if (next == delim) break;
            result += (char) next;
        }
        return result;
    }

    std::string getwithdelim(std::string& delims) {
        std::string result;
        while (1) {
            int next = getc();
            if (next == EOF) break;
            result += (char) next;
            for (char i : delims) if (next == i) break;
        }
        return result;
    }

    std::string getwithdelim(const char * delims) {
        std::string result;
        while (1) {
            int next = getc();
            if (next == EOF) break;
            result += (char) next;
            for (const char * i = delims; *i; i++) if (next == *i) break;
        }
        return result;
    }

    std::string getwithdelim(char delim) {
        std::string result;
        while (1) {
            int next = getc();
            if (next == EOF) break;
            result += (char) next;
            if (next == delim) break;
        }
        return result;
    }

    // Read the remaining bytes of the file into a string.
    std::string getall() {
        std::string result;
        while (1) {
            int next = getc();
            if (next == EOF) break;
            result += (char) next;
        }
        return result;
    }

    // Reads until encountering a newline ('\n'). The returned string does not
    // include the newline character. If this is desired, use
    // `getwithdelim('\n')` instead.
    std::string getline() { return getdelim('\n'); }

    // Reads until encountering an ASCII whitespace character, space as space,
    // newline, or tab. The returned string does not include the whitespace
    // character. If this is desired, use `getwithdelim(" \n\r\t\v\f")` instead.
    std::string getstring() { return getdelim(" \n\r\t\v\f"); }

    infile() {}
    infile(std::filesystem::path& path) { open(path); }
    infile(std::string& path) { open(path); }
    infile(const char * path) { open(path); }
    infile(FILE * f) { c_file = f; }
    ~infile() {if (is_open()) close();}
};

class boutfile : public file {
public:
    // Open the file at `path` for writing. Returns true if the file was
    // successfully opened.
    bool open(const char * path) {
        c_file = std::fopen(path, "wb");
        return c_file != nullptr;
    }
    bool open(std::filesystem::path& path) { return open(path.c_str()); }
    bool open(std::string& path) { return open(path.c_str()); }

    // Write a byte.
    void putc(char byte) { std::fputc(byte, c_file); }
    void write(std::vector<char>& buffer) { for (char i : buffer) putc(i); }
    void write(char * buffer, size_t s) { for (; s; s--) putc(*buffer++); }
    void flush() { std::fflush(c_file); }

    boutfile() {}
    boutfile(std::filesystem::path& path) { open(path); }
    boutfile(std::string& path) { open(path); }
    boutfile(char * path) { open(path); }
    boutfile(FILE * f) { c_file = f; }
    ~boutfile() {if (is_open()) close();}
};

class outfile : public boutfile {
public:
    // Open the file at `path` for writing. Returns true if the file was
    // successfully opened.
    bool open(const char * path) {
        c_file = std::fopen(path, "w");
        return c_file != nullptr;
    }
    bool open(std::filesystem::path& path) { return open(path.c_str()); }
    bool open(std::string& path) { return open(path.c_str()); }

    void puts(const char * s) { std::fputs(s, c_file); }
    void puts(std::string s) { puts(s.c_str()); }

    outfile() {}
    outfile(std::filesystem::path& path) { open(path); }
    outfile(std::string& path) { open(path); }
    outfile(const char * path) { open(path); }
    outfile(FILE * f) { c_file = f; }
    ~outfile() {if (is_open()) close();}
};

// Do not define aliases for the standard i/o streams if the user does not want
// them.
#ifndef FILE_UNDEF_STD
infile stdin = ::stdin;
outfile stdout = ::stdout;
outfile stderr = ::stderr;
#endif

}

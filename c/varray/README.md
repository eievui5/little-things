# VArray

I've said before that one of the only reasons I use C++ is for std::string,
std::vector, and std::unordered_map. This tiny library aims to make vectors and
strings more convienient to use in C.

This library has been made as lightweight as possible. There is no compiled
code, just a collection of macros and static inline functions. Ultimately it is
able to produce very small and fast output while abstracting away the logic
behind variably-sized arrays.

The "V" in "VArray" can be interpreted as either "variable" or "vector",
whichever you prefer to read it as :)
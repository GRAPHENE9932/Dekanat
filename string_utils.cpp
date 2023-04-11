#include "string_utils.hpp"

void trim_left(std::string& string, const std::string& redunant) {
    string.erase(0, string.find_first_not_of(redunant));
}

void trim_right(std::string& string, const std::string& redunant) {
    string.erase(string.find_last_not_of(redunant) + 1);
}

void trim(std::string& string, const std::string& redunant) {
    trim_right(string);
    trim_left(string);
}
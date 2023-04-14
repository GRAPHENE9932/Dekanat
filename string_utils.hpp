#pragma once

#include <string>
#include <vector>
#include <optional>

[[nodiscard]] inline bool is_english_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

[[nodiscard]] inline bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

const std::string whitespaces {" \t\n\r\f\v"};

void trim_left(std::string& string, const std::string& redunant = whitespaces);
void trim_right(std::string& string, const std::string& redunant = whitespaces);
void trim(std::string& string, const std::string& redunant = whitespaces);

[[nodiscard]] bool has_errors(const std::vector<std::optional<std::string>>& errors);
[[nodiscard]] std::string unite_errors(const std::vector<std::optional<std::string>>& errors);
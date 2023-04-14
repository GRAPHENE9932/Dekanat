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

[[nodiscard]] bool has_errors(const std::vector<std::optional<std::string>>& errors) {
    for (const auto& error : errors) {
        if (error.has_value()) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] std::string unite_errors(const std::vector<std::optional<std::string>>& errors) {
    std::string result;

    for (const auto& error : errors) {
        if (!error.has_value()) {
            continue;
        }

        result += *error;
        result += "<br/>";
    }

    return result;
}
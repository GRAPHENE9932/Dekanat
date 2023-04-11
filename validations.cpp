#include "validations.hpp"
#include "string_utils.hpp"
#include "main_database_manager.hpp"

#include <fstream>

[[nodiscard]] std::optional<std::string> validate_username(const std::string& username) {
    if (username.empty()) {
        return "Empty user name.";
    }

    if (username.length() > 512) {
        return "User name is too long.";
    }

    // Check if all characters are either English letters or spaces.
    for (const char c : username) {
        if (!is_english_letter(c) && c != ' ') {
            return "User name contains invalid characters. English letters and spaces are allowed only.";
        }
    }

    return std::nullopt;
}

[[nodiscard]] std::optional<std::string> validate_password_for_signup(const std::string& password, const std::string& repeat_password) {
    if (password != repeat_password) {
        return "Passwords do not match.";
    }

    if (password.length() < 8) {
        return "The password is too short (must be 8 characters long or longer).";
    }

    if (password.length() > 255) {
        return "The password is too long (must be less that 256 characters long).";
    }

    // Check if this password consists from the repeating characters.
    char first_char = password[0];
    bool same_chars = true;
    for (const char c : password) {
        if (c != first_char) {
            same_chars = false;
            break;
        }
    }
    if (same_chars) {
        return "The password is too weak.";
    }

    return std::nullopt;
}

[[nodiscard]] std::optional<std::string> validate_email_for_signup(const std::string& email) {
    if (email.empty()) {
        return "The specified E-mail is empty.";
    }

    if (check_if_has_admin(email)) {
        return "An account with this E-mail is already taken.";
    }

    return std::nullopt;
}

// Returns error, if the specified admin code is invalid.
[[nodiscard]] std::optional<std::string> validate_admin_code(const std::string& admin_code) {
    if (admin_code.empty()) {
        return "The specified administrator code is empty.";
    }

    // Get the correct admin code.
    std::ifstream stream("static/admin-code.txt");
    if (!stream) {
        return "There are no correct administrator code! Failed to read one from file.";
    }
    std::string correct_admin_code;
    stream >> correct_admin_code;
    if (admin_code != correct_admin_code) {
        return "Incorrect administrator code.";
    }

    return std::nullopt;
}


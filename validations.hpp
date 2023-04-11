#pragma once

#include <string>
#include <optional>

// Returns error, if the specified username is invalid.
[[nodiscard]] std::optional<std::string> validate_username(const std::string& username);
// Returns error, if the specified password is invalid.
[[nodiscard]] std::optional<std::string> validate_password(const std::string& password, const std::string& repeat_password);
// Returns error, if the specified E-mail is invalid or already taken.
[[nodiscard]] std::optional<std::string> validate_email_for_signup(const std::string& email);
// Returns error, if the specified admin code is invalid.
[[nodiscard]] std::optional<std::string> validate_admin_code(const std::string& admin_code);
#pragma once

#include <string>

void add_admin(const std::string& username, const std::string& email, const std::string& password);
[[nodiscard]] bool check_if_already_has_admin(const std::string& email);
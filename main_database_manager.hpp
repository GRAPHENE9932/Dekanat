#pragma once

#include <string>

void add_admin(const std::string& username, const std::string& email, const std::string& password);
void add_student(
    const std::string& username, const std::string& email,
    const std::string& password, const std::string& group_name
);
void add_group(const std::string& group_name);
[[nodiscard]] bool check_if_has_admin(const std::string& email);
[[nodiscard]] bool check_if_has_student(const std::string& email);
[[nodiscard]] bool validate_admin_credentials(const std::string& email, const std::string& password);
[[nodiscard]] bool validate_student_credentials(const std::string& email, const std::string& password);
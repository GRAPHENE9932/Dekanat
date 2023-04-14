#pragma once

#include <drogon/Session.h>

#include <string>
#include <vector>
#include <optional>

void add_admin(const std::string& username, const std::string& email, const std::string& password);
void add_student(
    const std::string& username, const std::string& email,
    const std::string& password, const std::string& group_name
);
void add_group(const std::string& group_name);
[[nodiscard]] bool check_if_has_admin(const std::string& email);
[[nodiscard]] bool check_if_has_student(const std::string& email);
[[nodiscard]] bool validate_admin_session(const drogon::SessionPtr& session);
[[nodiscard]] bool validate_student_or_admin_session(const drogon::SessionPtr& session);
[[nodiscard]] bool validate_admin_credentials(const std::string& email, const std::string& password);
[[nodiscard]] bool validate_student_credentials(const std::string& email, const std::string& password);
[[nodiscard]] std::vector<std::string> get_groups();
[[nodiscard]] bool group_exists(const std::string& group_name);
[[nodiscard]] std::optional<std::string> get_student_group(const std::string& email);
// Returns pair of the student name (first) and student email (second).
[[nodiscard]] std::vector<std::pair<std::string, std::string>> get_students_in_group(const std::string& group);
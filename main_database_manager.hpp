#pragma once

#include <drogon/Session.h>

#include <string>
#include <vector>
#include <optional>

namespace main_db {
    struct StudentData {
        std::string name;
        std::string email;
        std::string group;
    };

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
    [[nodiscard]] std::vector<StudentData> get_students_in_group(const std::string& group);
    void delete_admin(const std::string& email);
    void delete_student(const std::string& email);
    void delete_group(const std::string& name);
    [[nodiscard]] std::vector<StudentData> search_students(const std::string& query);
}
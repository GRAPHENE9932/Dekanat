#pragma once

#include "models/User.hpp"
#include "models/Group.hpp"

#include <drogon/drogon.h>

#include <optional>

class Student : public User {
public:
    Student(
        std::string_view name,
        std::string_view email,
        std::string_view group
    ) : User(name, email), group(group) {}

    inline void set_group(const Group& group) {
        this->group = group;
    }

    [[nodiscard]] inline const Group& get_group() const {
        return group;
    }

    void add_to_database() override;
    void remove_from_database() override;

    void save_to_session(drogon::Session& session, std::string_view password_cleartext) override;
    
    [[nodiscard]] static std::optional<Student> get_from_email(std::string_view email);
    [[nodiscard]] static std::optional<Student> get_from_session(const drogon::Session& session);
    [[nodiscard]] static std::vector<Student> get_students_in_group(const Group& group);
    [[nodiscard]] static std::vector<Student> search_students(std::string_view query);
    static void remove_all_from_group(const Group& group);

private:
    Group group;
    inline static bool student_table_was_created = false;
    static void create_student_table_if_dont_exist();
};
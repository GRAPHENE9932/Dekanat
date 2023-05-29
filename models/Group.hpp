#pragma once

#include <string>
#include <memory>
#include <vector>

class Student;

class Group {
public:
    explicit Group(std::string_view name) : name(name) {}

    inline void set_name(std::string_view name) {
        this->name = name;
    }
    [[nodiscard]] inline const std::string& get_name() const {
        return name;
    }

    void add_to_database() const;
    void remove_from_database() const;

    [[nodiscard]] static std::vector<Group> get_all_groups();

private:
    std::string name;
    inline static bool group_table_was_created = false;

    static void create_group_table_if_dont_exist();
};
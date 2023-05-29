#include "Group.hpp"
#include "Student.hpp"

void Group::create_group_table_if_dont_exist() {
    if (group_table_was_created) {
        return;
    }

    auto client = drogon::app().getDbClient("main");
    client->execSqlSync(
        "CREATE TABLE IF NOT EXISTS groups (name TEXT)"
    );

    group_table_was_created = true;
}

void Group::add_to_database() const {
    create_group_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");
    client->execSqlSync(
        "INSERT INTO groups (name) VALUES (?)", name
    );
}

void Group::remove_from_database() const {
    create_group_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    client->execSqlSync("DELETE FROM students WHERE `group`=?", name);
    client->execSqlSync("DELETE FROM groups WHERE name=?", name);
}

[[nodiscard]] std::vector<Group> Group::get_all_groups() {
    create_group_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    auto result = client->execSqlSync("SELECT * FROM groups");
    std::vector<Group> groups;
    groups.reserve(result.size());
    for (const auto& row : result) {
        groups.emplace_back(std::move(row.at("name").as<std::string>()));
    }

    return groups;
}
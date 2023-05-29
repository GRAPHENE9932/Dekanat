#include "Student.hpp"

#include <drogon/drogon.h>

#include <vector>

void Student::create_student_table_if_dont_exist() {
    if (student_table_was_created) {
        return;
    }

    auto client = drogon::app().getDbClient("main");
    client->execSqlSync(
        "CREATE TABLE IF NOT EXISTS students (email TEXT, username TEXT, 'group' TEXT, password_hash BLOB, salt BLOB)"
    );

    student_table_was_created = true;
}

void Student::add_to_database() {
    create_student_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");
    client->execSqlSync(
        "INSERT INTO students (email, username, 'group', password_hash, salt)"
        "VALUES (?, ?, ?, ?, ?)",
        get_email(), get_name(), get_group().get_name(),
        std::vector<char>(password_hash.begin(), password_hash.end()),
        std::vector<char>(password_salt.begin(), password_salt.end())
    );
}

void Student::remove_from_database() {
    create_student_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    client->execSqlSync("DELETE FROM students WHERE email=?", get_email());
}

void Student::save_to_session(drogon::Session& session, std::string_view password_cleartext) {
    session.insert("email", std::string(email));
    session.insert("password", std::string(password_cleartext));
}

static Student get_from_row(const drogon::orm::Row& row) {
    Student result(
        row.at("username").as<std::string>(),
        row.at("email").as<std::string>(),
        row.at("group").as<std::string>()
    );
    const auto& hash = row.at("password_hash").as<std::vector<char>>();
    result.set_password_hash(hash.begin(), hash.end());
    const auto& salt = row.at("salt").as<std::vector<char>>();
    result.set_password_salt(salt.begin(), salt.end());

    return result;
}

std::optional<Student> Student::get_from_email(std::string_view email) {
    create_student_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    auto result = client->execSqlSync("SELECT * FROM students WHERE email=?", email);
    if (result.empty()) {
        return std::nullopt;
    }

    return get_from_row(result[0]);
}

std::optional<Student> Student::get_from_session(const drogon::Session& session) {
    auto email = session.getOptional<std::string>("email");
    if (!email.has_value()) {
        return std::nullopt;
    }

    auto password = session.getOptional<std::string>("password");
    if (!password.has_value()) {
        return std::nullopt;
    }

    auto student = get_from_email(*email);
    if (student.has_value() && student->check_password(*password)) {
        return student;
    }
    else {
        return std::nullopt;
    }
}

[[nodiscard]] std::vector<Student> Student::get_students_in_group(const Group& group) {
    create_student_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    auto result = client->execSqlSync("SELECT * FROM students WHERE \"group\"=?", group.get_name());
    
    std::vector<Student> output;
    output.reserve(result.size());
    for (const auto& row : result) {
        output.push_back(get_from_row(row));
    }

    return output;
}

[[nodiscard]] std::vector<Student> Student::search_students(std::string_view query) {
    create_student_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    std::string like_query = "%" + std::string(query) + "%";
    auto result = client->execSqlSync(
        "SELECT * FROM students "
        "WHERE username LIKE ? OR email LIKE ?",
        like_query, like_query
    );

    std::vector<Student> output;
    output.reserve(result.size());
    for (const auto& row : result) {
        output.push_back(get_from_row(row));
    }

    return output;
}

void Student::remove_all_from_group(const Group& group) {
    create_student_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    client->execSqlSync("DELETE FROM students WHERE `group`=?", group.get_name());
}
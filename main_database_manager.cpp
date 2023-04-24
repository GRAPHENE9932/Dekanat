#include "main_database_manager.hpp"
#include "random_generator.hpp"

#include <drogon/drogon.h>
#include <openssl/sha.h>

#include <random>
#include <span>

constexpr std::size_t SALT_SIZE = 16;

bool admin_table_was_created = false;
bool student_table_was_created = false;
bool group_table_was_created = false;

namespace main_db {
    void create_admin_table_if_dont_exist() {
        if (admin_table_was_created) {
            return;
        }

        auto client = drogon::app().getDbClient("main");
        client->execSqlSync(
            "CREATE TABLE IF NOT EXISTS admins (email TEXT, username TEXT, password_hash BLOB, salt BLOB)"
        );

        admin_table_was_created = true;
    }

    void create_student_table_if_dont_exist() {
        if (student_table_was_created) {
            return;
        }

        auto client = drogon::app().getDbClient("main");
        client->execSqlSync(
            "CREATE TABLE IF NOT EXISTS students (email TEXT, username TEXT, 'group' TEXT, password_hash BLOB, salt BLOB)"
        );

        student_table_was_created = true;
    }

    void create_group_table_if_dont_exist() {
        if (group_table_was_created) {
            return;
        }

        auto client = drogon::app().getDbClient("main");
        client->execSqlSync(
            "CREATE TABLE IF NOT EXISTS groups (name TEXT)"
        );

        group_table_was_created = true;
    }

    [[nodiscard]] std::vector<char> hash_bytes(std::span<char> data) {
        std::vector<char> result(512 / 8);
        SHA512(
            reinterpret_cast<const unsigned char*>(data.data()),
            data.size(),
            reinterpret_cast<unsigned char*>(result.data())
        );
        return result;
    }

    [[nodiscard]] std::vector<char> hash_and_salt_password(
        const std::string& password, std::span<char> salt
    ) {
        std::vector<char> salted_password(password.length() + SALT_SIZE);
        std::copy(password.begin(), password.end(), salted_password.begin());
        std::copy(salt.begin(), salt.end(), salted_password.begin() + password.length());

        return hash_bytes(salted_password);
    }

    // Returns hash (first) and salt (second).
    [[nodiscard]] std::pair<std::vector<char>, std::vector<char>> hash_and_salt_password(const std::string& password) {
        std::vector<char> salt(SALT_SIZE);
        generate_random_bytes(salt);

        auto hashed = hash_and_salt_password(password, salt);

        return {hashed, salt};
    }

    void add_admin(const std::string& username, const std::string& email, const std::string& password) {
        create_admin_table_if_dont_exist();

        auto hash_and_salt = hash_and_salt_password(password);

        auto client = drogon::app().getDbClient("main");
        client->execSqlSync(
            "INSERT INTO admins (email, username, password_hash, salt)"
            "VALUES (?, ?, ?, ?)",
            email, username, hash_and_salt.first, hash_and_salt.second
        );
    }

    void add_student(
        const std::string& username, const std::string& email,
        const std::string& password, const std::string& group_name
    ) {
        create_student_table_if_dont_exist();

        auto hash_and_salt = hash_and_salt_password(password);

        auto client = drogon::app().getDbClient("main");
        client->execSqlSync(
            "INSERT INTO students (email, username, 'group', password_hash, salt)"
            "VALUES (?, ?, ?, ?, ?)",
            email, username, group_name, hash_and_salt.first, hash_and_salt.second
        );
    }

    void add_group(const std::string& group_name) {
        create_group_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");
        client->execSqlSync(
            "INSERT INTO groups (name) VALUES (?)", group_name
        );
    }

    [[nodiscard]] bool check_if_has_admin(const std::string& email) {
        create_admin_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");
        auto result = client->execSqlSync(
            "SELECT * FROM admins WHERE email=?",
            email
        );

        return !result.empty();
    }

    [[nodiscard]] bool check_if_has_student(const std::string& email) {
        create_student_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");
        auto result = client->execSqlSync(
            "SELECT * FROM students WHERE email=?",
            email
        );

        return !result.empty();
    }

    [[nodiscard]] bool validate_admin_session(const drogon::SessionPtr& session) {
        auto email = session->getOptional<std::string>("email");
        auto password = session->getOptional<std::string>("password");
        if (email.has_value() && password.has_value()) {
            return validate_admin_credentials(*email, *password);
        }
        else {
            return false;
        }
    }

    [[nodiscard]] bool validate_student_or_admin_session(const drogon::SessionPtr& session) {
        auto email = session->getOptional<std::string>("email");
        auto password = session->getOptional<std::string>("password");
        if (email.has_value() && password.has_value()) {
            return validate_student_credentials(*email, *password) || validate_admin_credentials(*email, *password);
        }
        else {
            return false;
        }
    }

    [[nodiscard]] bool validate_admin_credentials(const std::string& email, const std::string& password) {
        create_admin_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        auto result = client->execSqlSync("SELECT * FROM admins WHERE email=?", email);
        if (result.empty()) {
            return false;
        }

        std::vector<char> salt = result.at(0).at("salt").as<std::vector<char>>();
        std::vector<char> salted_hash_1 = hash_and_salt_password(password, salt);
        std::vector<char> salted_hash_2 = result.at(0).at("password_hash").as<std::vector<char>>();
        if (salted_hash_1 != salted_hash_2) {
            return false;
        }

        return true;
    }

    [[nodiscard]] bool validate_student_credentials(const std::string& email, const std::string& password) {
        create_student_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        auto result = client->execSqlSync("SELECT * FROM students WHERE email=?", email);
        if (result.empty()) {
            return false;
        }

        std::vector<char> salt = result.at(0).at("salt").as<std::vector<char>>();
        std::vector<char> salted_hash_1 = hash_and_salt_password(password, salt);
        std::vector<char> salted_hash_2 = result.at(0).at("password_hash").as<std::vector<char>>();
        if (salted_hash_1 != salted_hash_2) {
            return false;
        }

        return true;
    }

    [[nodiscard]] std::vector<std::string> get_groups() {
        create_group_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        auto result = client->execSqlSync("SELECT * FROM groups");
        std::vector<std::string> groups;
        groups.reserve(result.size());
        for (const auto& row : result) {
            groups.emplace_back(std::move(row.at("name").as<std::string>()));
        }

        return groups;
    }

    [[nodiscard]] bool group_exists(const std::string& group_name) {
        create_group_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        auto result = client->execSqlSync("SELECT * FROM groups WHERE 'group'=?", group_name);
        return !result.empty();
    }

    [[nodiscard]] std::optional<std::string> get_student_group(const std::string& email) {
        create_student_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        auto result = client->execSqlSync("SELECT * FROM students WHERE email=?", email);
        if (result.empty()) {
            return std::nullopt;
        }

        return result.at(0).at("group").as<std::string>();
    }

    [[nodiscard]] std::vector<StudentData> get_students_in_group(const std::string& group) {
        create_student_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        auto result = client->execSqlSync("SELECT * FROM students WHERE \"group\"=?", group);
        
        std::vector<StudentData> output;
        output.reserve(result.size());
        for (const auto& row : result) {
            output.push_back({
                std::move(row.at("username").as<std::string>()),
                std::move(row.at("email").as<std::string>()),
                std::move(row.at("group").as<std::string>())
            });
        }

        return output;
    }

    void delete_admin(const std::string& email) {
        create_admin_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        client->execSqlSync("DELETE FROM admins WHERE email=?", email);
    }

    void delete_student(const std::string &email) {
        create_student_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        client->execSqlSync("DELETE FROM students WHERE email=?", email);
    }

    [[nodiscard]] std::vector<StudentData> search_students(const std::string& query) {
        create_student_table_if_dont_exist();

        auto client = drogon::app().getDbClient("main");

        std::string like_query = "%" + query + "%";
        auto result = client->execSqlSync(
            "SELECT * FROM students "
            "WHERE username LIKE ? OR email LIKE ?",
            like_query, like_query
        );

        std::vector<StudentData> output;
        output.reserve(result.size());
        for (const auto& row : result) {
            output.push_back({
                std::move(row.at("username").as<std::string>()),
                std::move(row.at("email").as<std::string>()),
                std::move(row.at("group").as<std::string>())
            });
        }

        return output;
    }
}
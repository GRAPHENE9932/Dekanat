#include "main_database_manager.hpp"
#include "random_generator.hpp"

#include <drogon/drogon.h>
#include <openssl/sha.h>

#include <random>
#include <span>

constexpr std::size_t SALT_SIZE = 16;

bool admin_table_was_created = false;
std::mt19937_64 random_generator();

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

[[nodiscard]] std::vector<char> hash_bytes(std::span<char> data) {
    std::vector<char> result(512 / 8);
    SHA512(
        reinterpret_cast<const unsigned char*>(data.data()),
        data.size(),
        reinterpret_cast<unsigned char*>(result.data())
    );
    return result;
}

// Returns hash (first) and salt (second).
[[nodiscard]] std::pair<std::vector<char>, std::vector<char>> hash_and_salt_password(const std::string& password) {
    std::vector<char> salt(SALT_SIZE);
    generate_random_bytes(salt);

    std::vector<char> salted_password(password.length() + SALT_SIZE);
    std::copy(password.begin(), password.end(), salted_password.begin());
    std::copy(salt.begin(), salt.end(), salted_password.begin() + password.length());

    return {hash_bytes(salted_password), salt};
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

[[nodiscard]] bool check_if_has_admin(const std::string& email) {
    create_admin_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");
    auto result = client->execSqlSync(
        "SELECT * FROM admins WHERE email=?",
        email
    );

    return !result.empty();
}
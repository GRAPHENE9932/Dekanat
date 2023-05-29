#include "models/Admin.hpp"

void Admin::create_admin_table_if_dont_exist() {
    if (admin_table_was_created) {
        return;
    }

    auto client = drogon::app().getDbClient("main");
    client->execSqlSync(
        "CREATE TABLE IF NOT EXISTS admins (email TEXT, username TEXT, password_hash BLOB, salt BLOB)"
    );

    admin_table_was_created = true;
}

void Admin::add_to_database() {
    create_admin_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");
    client->execSqlSync(
        "INSERT INTO admins (email, username, password_hash, salt)"
        "VALUES (?, ?, ?, ?)",
        get_email(), get_name(),
        std::vector<char>(password_hash.begin(), password_hash.end()),
        std::vector<char>(password_salt.begin(), password_salt.end())
    );
}

void Admin::save_to_session(drogon::Session& session, std::string_view password_cleartext) {
    session.insert("email", std::string(email));
    session.insert("password", std::string(password_cleartext));
}

void Admin::remove_from_database() {
    create_admin_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    client->execSqlSync("DELETE FROM admins WHERE email=?", get_email());
}

static Admin get_from_row(const drogon::orm::Row& row) {
    Admin result(
        row.at("username").as<std::string>(),
        row.at("email").as<std::string>()
    );
    const auto& hash = row.at("password_hash").as<std::vector<char>>();
    result.set_password_hash(hash.begin(), hash.end());
    const auto& salt = row.at("salt").as<std::vector<char>>();
    result.set_password_salt(salt.begin(), salt.end());

    return result;
}

std::optional<Admin> Admin::get_from_email(std::string_view email) {
    create_admin_table_if_dont_exist();

    auto client = drogon::app().getDbClient("main");

    auto result = client->execSqlSync("SELECT * FROM admins WHERE email=?", email);
    if (result.empty()) {
        return std::nullopt;
    }

    return get_from_row(result[0]);
}

std::optional<Admin> Admin::get_from_session(const drogon::Session& session) {
    auto email = session.getOptional<std::string>("email");
    if (!email.has_value()) {
        return std::nullopt;
    }

    auto password = session.getOptional<std::string>("password");
    if (!password.has_value()) {
        return std::nullopt;
    }

    auto admin = get_from_email(*email);
    if (admin.has_value() && admin->check_password(*password)) {
        return admin;
    }
    else {
        return std::nullopt;
    }
}
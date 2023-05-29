#pragma once

#include "models/User.hpp"

#include <drogon/drogon.h>

#include <optional>

class Admin : public User {
public:
    Admin(
        std::string_view name,
        std::string_view email
    ) : User(name, email) {}

    void add_to_database() override;
    void remove_from_database() override;

    void save_to_session(drogon::Session& session, std::string_view password_cleartext) override;
    
    static std::optional<Admin> get_from_email(std::string_view email);
    static std::optional<Admin> get_from_session(const drogon::Session& session);

private:
    inline static bool admin_table_was_created = false;
    static void create_admin_table_if_dont_exist();
};
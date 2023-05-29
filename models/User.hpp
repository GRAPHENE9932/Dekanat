#pragma once

#include <drogon/Session.h>

#include <array>
#include <string>

class User {
public:
    static constexpr std::size_t HASH_SIZE = 512 / 8; // SHA512 returns 512 bits.
    static constexpr std::size_t SALT_SIZE = 16;

    User(
        std::string_view name,
        std::string_view email
    ) : name(name), email(email) {}

    virtual ~User() = default;

    inline void set_name(std::string_view name) {
        this->name = name;
    }
    inline void set_email(std::string_view email) {
        this->email = email;
    }
    template<typename Iter>
    inline void set_password_hash(Iter begin, Iter end) {
        std::copy(begin, end, password_hash.begin());
    }
    template<typename Iter>
    inline void set_password_salt(Iter begin, Iter end) {
        std::copy(begin, end, password_salt.begin());
    }

    void set_password(std::string_view password);
    [[nodiscard]] bool check_password(std::string_view password) const;

    [[nodiscard]] inline const std::string& get_name() const noexcept {
        return name;
    }
    [[nodiscard]] inline const std::string& get_email() const noexcept {
        return email;
    }

    virtual void add_to_database() = 0;
    virtual void remove_from_database() = 0;

    virtual void save_to_session(drogon::Session& session, std::string_view password_cleartext) = 0;

protected:
    std::string name;
    std::string email;
    std::array<char, HASH_SIZE> password_hash;
    std::array<char, SALT_SIZE> password_salt;
};
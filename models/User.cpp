#include "User.hpp"
#include "random_generator.hpp"

#include <openssl/sha.h>

#include <vector>

[[nodiscard]] std::array<char, User::HASH_SIZE> hash_bytes(std::span<char> data) {
    std::array<char, User::HASH_SIZE> result;
    SHA512(
        reinterpret_cast<const unsigned char*>(data.data()),
        data.size(),
        reinterpret_cast<unsigned char*>(result.data())
    );
    return result;
}

template<typename Iter>
[[nodiscard]] std::array<char, User::HASH_SIZE> hash_and_salt_password(
    std::string_view password, Iter salt_begin, Iter salt_end
) {
    std::vector<char> salted_password(password.length() + User::SALT_SIZE);
    std::copy(password.begin(), password.end(), salted_password.begin());
    std::copy(salt_begin, salt_end, salted_password.begin() + password.length());

    return hash_bytes(salted_password);
}

// Returns hash (first) and salt (second).
[[nodiscard]] std::pair<std::array<char, User::HASH_SIZE>, std::array<char, User::SALT_SIZE>>
hash_and_salt_password(std::string_view password) {
    std::array<char, User::SALT_SIZE> salt;
    generate_random_bytes(salt);

    auto hashed = hash_and_salt_password(password, salt.begin(), salt.end());

    return {hashed, salt};
}

void User::set_password(std::string_view password) {
    auto hash_and_salt = hash_and_salt_password(password);
    this->password_hash = hash_and_salt.first;
    this->password_salt = hash_and_salt.second;
}

[[nodiscard]] bool User::check_password(std::string_view password) const {
    auto hash = hash_and_salt_password(password, password_salt.begin(), password_salt.end());
    return hash == password_hash;
}
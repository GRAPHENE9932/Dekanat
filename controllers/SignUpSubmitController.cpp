#include "SignUpSubmitController.hpp"
#include "string_utils.hpp"

#include <regex>

// Returns error, if the specified username is invalid.
[[nodiscard]] std::optional<std::string> validate_username(const std::string& username) {
    if (username.empty()) {
        return "Empty user name.";
    }

    if (username.length() > 512) {
        return "User name is too long.";
    }

    // Check if all characters are either English letters or spaces.
    for (const char c : username) {
        if (!is_english_letter(c) && c != ' ') {
            return "User name contains invalid characters. English letters and spaces are allowed only.";
        }
    }

    return std::nullopt;
}

// Returns error, if the specified password is invalid.
[[nodiscard]] std::optional<std::string> validate_password(const std::string& password, const std::string& repeat_password) {
    if (password != repeat_password) {
        return "Passwords do not match.";
    }

    if (password.length() < 8) {
        return "The password is too short (must be 8 characters long or longer).";
    }

    if (password.length() > 255) {
        return "The password is too long (must be less that 256 characters long).";
    }

    // Check if this password consists from the repeating characters.
    char first_char = password[0];
    bool same_chars = true;
    for (const char c : password) {
        if (c != first_char) {
            same_chars = false;
            break;
        }
    }
    if (same_chars) {
        return "The password is too weak.";
    }

    return std::nullopt;
}

// Returns error, if the specified E-mail is invalid or already taken.
[[nodiscard]] std::optional<std::string> validate_email(const std::string& email) {
    if (email.empty()) {
        return "The specified E-mail is empty.";
    }

    return std::nullopt;
}

// Returns error, if the specified admin code is invalid.
[[nodiscard]] std::optional<std::string> validate_admin_code(const std::string& admin_code) {
    return std::nullopt;
}

[[nodiscard]] bool has_errors(const std::vector<std::optional<std::string>>& errors) {
    for (const auto& error : errors) {
        if (error.has_value()) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] std::string unite_errors(const std::vector<std::optional<std::string>>& errors) {
    std::string result;

    for (const auto& error : errors) {
        if (!error.has_value()) {
            continue;
        }

        result += *error;
        result += "<br/>";
    }

    return result;
}

void SignUpSubmitController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    std::vector<std::optional<std::string>> errors;

    // Get and validate username.
    std::string username = request->getParameter("username");
    trim(username);
    errors.push_back(validate_username(username));

    // Get and validate password.
    std::string password = request->getParameter("password");
    std::string repeat_password = request->getParameter("repeat-password");
    errors.push_back(validate_password(password, repeat_password));

    // Get and validate email.
    std::string email = request->getParameter("email");
    trim(email);
    errors.push_back(validate_email(email));

    // Get and validate admin code.
    std::string admin_code = request->getParameter("admin-code");
    errors.push_back(validate_admin_code(admin_code));

    if (has_errors(errors)) {
        std::string errors_str {unite_errors(errors)};
        drogon::HttpViewData view_data;
        view_data.insert("errors_str", errors_str);
        auto response = drogon::HttpResponse::newHttpViewResponse("SignUpPage.csp", view_data);
        callback(response);
    }
    else {
        auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
        callback(response);
    }
}

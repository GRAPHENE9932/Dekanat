#include "SignUpController.hpp"
#include "string_utils.hpp"
#include "validations.hpp"
#include "models/Admin.hpp"

void SignUpController::submit(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback
) {
    std::vector<std::optional<std::string>> errors;

    // Get and validate username.
    std::string username = request->getParameter("username");
    trim(username);
    errors.push_back(validate_username(username));

    // Get and validate password.
    std::string password = request->getParameter("password");
    std::string repeat_password = request->getParameter("repeat-password");
    errors.push_back(validate_password_for_signup(password, repeat_password));

    // Get and validate email.
    std::string email = request->getParameter("email");
    trim(email);
    errors.push_back(validate_email_for_signup(email));

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
        Admin admin(username, email);
        admin.set_password(password);
        admin.add_to_database();
        admin.save_to_session(*request->session(), password);

        auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
        callback(response);
    }
}

void SignUpController::show_page(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback
) {
    auto response = drogon::HttpResponse::newHttpViewResponse("SignUpPage.csp");
    callback(response);
}
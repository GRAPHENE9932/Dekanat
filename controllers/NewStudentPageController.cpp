#include "NewStudentPageController.hpp"
#include "main_database_manager.hpp"
#include "string_utils.hpp"
#include "validations.hpp"

void NewStudentPageController::submit(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback,
    const std::string& group
) {
    if (!main_db::validate_admin_session(request->getSession())) {
        auto response = drogon::HttpResponse::newRedirectionResponse("/login");
        callback(response);
        return;
    }

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

    drogon::HttpViewData view_data;
    view_data.insert("group", group);
    if (has_errors(errors)) {
        std::string errors_str {unite_errors(errors)};
        view_data.insert("errors_str", errors_str);

        auto response = drogon::HttpResponse::newHttpViewResponse("NewStudentPage.csp", view_data);
        callback(response);
    }
    else {
        main_db::add_student(username, email, password, group);

        auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
        callback(response);
    }
}

void NewStudentPageController::show_page(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback,
    const std::string& group
) {
    if (!main_db::validate_admin_session(request->getSession())) {
        auto response = drogon::HttpResponse::newRedirectionResponse("/login");
        callback(response);
        return;
    }

    drogon::HttpViewData view_data;
    view_data.insert("group", group);
    auto response = drogon::HttpResponse::newHttpViewResponse("NewStudentPage.csp", view_data);
    callback(response);
}
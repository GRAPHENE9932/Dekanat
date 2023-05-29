#include "LogInController.hpp"
#include "models/Student.hpp"
#include "models/Admin.hpp"
#include "string_utils.hpp"
#include <drogon/HttpResponse.h>

void LogInController::submit(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    std::vector<std::optional<std::string>> errors;

    std::string password = request->getParameter("password");
    std::string email = request->getParameter("email");
    trim(email);

    if (
        Admin::get_from_session(*request->getSession()).has_value() ||
        Student::get_from_session(*request->getSession()).has_value()
    ) {
        request->session()->insert("email", email);
        request->session()->insert("password", password);
        auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
        callback(response);
    }
    else {
        drogon::HttpViewData view_data;
        view_data.insert("errors_str", std::string("Incorrect E-mail or password."));
        auto response = drogon::HttpResponse::newHttpViewResponse("LogInPage.csp", view_data);
        callback(response);
    }
}

void LogInController::show_page(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback
) {
    if (request->session()->find("email") && request->session()->find("password")) {
        std::string email = request->session()->get<std::string>("email");
        std::string password = request->session()->get<std::string>("password");
        if (
            Admin::get_from_session(*request->getSession()).has_value() ||
            Student::get_from_session(*request->getSession()).has_value())
        {
            auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
            callback(response);
        }
    }
    else {
        auto response = drogon::HttpResponse::newHttpViewResponse("LogInPage.csp");
        callback(response);
    }
}

void LogInController::logout(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback
) {
    request->session()->erase("email");
    request->session()->erase("password");

    auto response = drogon::HttpResponse::newRedirectionResponse("/");
    callback(response);
}
#include "LogInPageController.hpp"
#include "main_database_manager.hpp"

void LogInPageController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    if (request->session()->find("email") && request->session()->find("password")) {
        std::string email = request->session()->get<std::string>("email");
        std::string password = request->session()->get<std::string>("password");
        if (validate_student_credentials(email, password) || validate_admin_credentials(email, password)) {
            auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
            callback(response);
        }
    }
    else {
        auto response = drogon::HttpResponse::newHttpViewResponse("LogInPage.csp");
        callback(response);
    }
}
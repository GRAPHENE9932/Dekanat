#include "LogInSubmitController.hpp"
#include "main_database_manager.hpp"
#include "string_utils.hpp"
#include "validations.hpp"

void LogInSubmitController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    std::vector<std::optional<std::string>> errors;

    std::string password = request->getParameter("password");
    std::string email = request->getParameter("email");
    trim(email);

    if (validate_student_credentials(email, password) || validate_admin_credentials(email, password)) {
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
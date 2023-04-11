#include "GroupPageController.hpp"
#include "main_database_manager.hpp"

void GroupPageController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    const auto& group = request->getOptionalParameter<std::string>("name");
    if (!group.has_value()) {
        auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
        callback(response);
        return;
    }

    if (!request->session()->find("email") || !request->session()->find("password")) {
        auto response = drogon::HttpResponse::newHttpViewResponse("LogInPage.csp");
        callback(response);
        return;
    }

    std::string email = request->session()->get<std::string>("email");
    std::string password = request->session()->get<std::string>("password");

    drogon::HttpViewData view_data;
    view_data.insert("students", get_students_in_group(*group));
    if (validate_student_credentials(email, password)) {
        view_data.insert("is_admin", false);
    }
    else if (validate_admin_credentials(email, password)) {
        view_data.insert("is_admin", true);
    }
    else {
        auto response = drogon::HttpResponse::newHttpViewResponse("LogInPage.csp");
        callback(response);
        return;
    }
    
    auto response = drogon::HttpResponse::newHttpViewResponse("GroupPage.csp", view_data);
    callback(response);
}
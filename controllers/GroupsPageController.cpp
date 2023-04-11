#include "GroupsPageController.hpp"
#include "main_database_manager.hpp"

void GroupsPageController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    if (request->session()->find("email") && request->session()->find("password")) {
        std::string email = request->session()->get<std::string>("email");
        std::string password = request->session()->get<std::string>("password");

        if (validate_student_credentials(email, password)) {
            drogon::HttpViewData view_data;
            view_data.insert("is_admin", false);
            view_data.insert("groups", get_groups());
            view_data.insert("my_group", get_student_group(email));
            auto response = drogon::HttpResponse::newHttpViewResponse("GroupsPage.csp", view_data);
            callback(response);
        }
        else if (validate_admin_credentials(email, password)) {
            drogon::HttpViewData view_data;
            view_data.insert("is_admin", true);
            view_data.insert("groups", get_groups());
            auto response = drogon::HttpResponse::newHttpViewResponse("GroupsPage.csp", view_data);
            callback(response);
        }
    }
    else {
        auto response = drogon::HttpResponse::newHttpViewResponse("LogInPage.csp");
        callback(response);
    }
}
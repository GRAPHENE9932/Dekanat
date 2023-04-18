#include "GroupPageController.hpp"
#include "main_database_manager.hpp"
#include <drogon/HttpResponse.h>

void GroupPageController::show(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback,
    const std::string& name
) {
    if (name.empty()) {
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
    view_data.insert("students", main_db::get_students_in_group(name));
    view_data.insert("group", name);
    if (main_db::validate_student_credentials(email, password)) {
        view_data.insert("is_admin", false);
    }
    else if (main_db::validate_admin_credentials(email, password)) {
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

void GroupPageController::delete_student(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback,
    const std::string& group,
    const std::string& email
) {
    if (!main_db::validate_admin_session(request->getSession())) {
        auto response = drogon::HttpResponse::newNotFoundResponse();
        callback(response);
        return;
    }

    if (email.empty() || group.empty()) {
        auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
        callback(response);
        return;
    }
    
    main_db::delete_student(email);

    auto response = drogon::HttpResponse::newRedirectionResponse("/group?name=" + group);
    callback(response);
}
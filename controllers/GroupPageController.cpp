#include "GroupPageController.hpp"
#include "models/Group.hpp"
#include "models/Student.hpp"
#include "models/Admin.hpp"
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

    Group group(name);

    drogon::HttpViewData view_data;
    view_data.insert("students", Student::get_students_in_group(group));
    view_data.insert("group_name", group.get_name());
    if (Student::get_from_session(*request->getSession()).has_value()) {
        view_data.insert("is_admin", false);
    }
    else if (Admin::get_from_session(*request->getSession()).has_value()) {
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
    if (!Admin::get_from_session(*request->getSession()).has_value()) {
        auto response = drogon::HttpResponse::newNotFoundResponse();
        callback(response);
        return;
    }

    if (email.empty() || group.empty()) {
        auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
        callback(response);
        return;
    }
    
    Student::get_from_email(email)->remove_from_database();

    auto response = drogon::HttpResponse::newRedirectionResponse("/group?name=" + group);
    callback(response);
}
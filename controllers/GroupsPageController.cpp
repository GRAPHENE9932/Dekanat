#include "GroupsPageController.hpp"
#include "models/Student.hpp"
#include "models/Admin.hpp"

void GroupsPageController::show_page(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback
) {
    if (request->session()->find("email") && request->session()->find("password")) {
        std::string email = request->session()->get<std::string>("email");
        std::string password = request->session()->get<std::string>("password");

        auto student {Student::get_from_session(*request->getSession())};
        if (student.has_value()) {
            Group my_group = student->get_group();

            drogon::HttpViewData view_data;
            view_data.insert("is_admin", false);
            view_data.insert("groups", Group::get_all_groups());
            view_data.insert("my_group", my_group);
            auto response = drogon::HttpResponse::newHttpViewResponse("GroupsPage.csp", view_data);
            callback(response);
        }
        else if (Admin::get_from_session(*request->getSession()).has_value()) {
            drogon::HttpViewData view_data;
            view_data.insert("is_admin", true);
            view_data.insert("groups", Group::get_all_groups());
            auto response = drogon::HttpResponse::newHttpViewResponse("GroupsPage.csp", view_data);
            callback(response);
        }
    }
    else {
        auto response = drogon::HttpResponse::newHttpViewResponse("LogInPage.csp");
        callback(response);
    }
}

void GroupsPageController::delete_group(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback,
    const std::string& name
) {
    if (!Admin::get_from_session(*request->getSession()).has_value()) {
        auto response = drogon::HttpResponse::newRedirectionResponse("/login");
        callback(response);
        return;
    }

    if (!name.empty()) {
        Group group(name);
        group.remove_from_database();
    }

    auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
    callback(response);
    return;
}
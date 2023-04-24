#include "SearchPageController.hpp"
#include "main_database_manager.hpp"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

void SearchPageController::show_page(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback,
    const std::string& query
) {
    if (!main_db::validate_student_or_admin_session(request->getSession())) {
        auto response = drogon::HttpResponse::newRedirectionResponse("/login");
        callback(response);
        return;
    }

    if (query.empty()) {
        auto response = drogon::HttpResponse::newHttpViewResponse("SearchPage.csp");
        callback(response);
        return;
    }

    auto view_data = drogon::HttpViewData();
    view_data.insert("students", main_db::search_students(query));
    auto response = drogon::HttpResponse::newHttpViewResponse("SearchPage.csp", view_data);
    callback(response);
}
#include "SearchPageController.hpp"
#include "models/Student.hpp"
#include "models/Admin.hpp"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

void SearchPageController::show_page(
    const drogon::HttpRequestPtr& request,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback,
    const std::string& query
) {
    if (
        Student::get_from_session(*request->getSession()).has_value() ||
        Admin::get_from_session(*request->getSession()).has_value()
    ) {
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
    view_data.insert("students", Student::search_students(query));
    auto response = drogon::HttpResponse::newHttpViewResponse("SearchPage.csp", view_data);
    callback(response);
}
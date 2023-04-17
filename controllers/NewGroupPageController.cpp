#include "NewGroupPageController.hpp"
#include "main_database_manager.hpp"
#include "validations.hpp"
#include <drogon/HttpResponse.h>

void NewGroupPageController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    if (!validate_admin_session(request->getSession())) {
        auto response = drogon::HttpResponse::newNotFoundResponse();
        callback(response);
        return;
    }

    auto submitted_name = request->getOptionalParameter<std::string>("new-name");
    if (submitted_name.has_value()) {
        auto error = validate_group_name(*submitted_name);
        if (error.has_value()) {
            drogon::HttpViewData view_data;
            view_data.insert("errors_str", *error);
            auto response = drogon::HttpResponse::newHttpViewResponse("NewGroupPage.csp", view_data);
            callback(response);
        }
        else {
            add_group(*submitted_name);
            auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
            callback(response);
        }
    }
    else {
        auto response = drogon::HttpResponse::newHttpViewResponse("NewGroupPage.csp");
        callback(response);
    }
}

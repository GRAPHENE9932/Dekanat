#include "NewGroupPageController.hpp"
#include "models/Admin.hpp"
#include "models/Group.hpp"
#include "validations.hpp"
#include <drogon/HttpResponse.h>

void NewGroupPageController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    if (!Admin::get_from_session(*request->getSession()).has_value()) {
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
            Group group(*submitted_name);
            group.add_to_database();
            auto response = drogon::HttpResponse::newRedirectionResponse("/groups");
            callback(response);
        }
    }
    else {
        auto response = drogon::HttpResponse::newHttpViewResponse("NewGroupPage.csp");
        callback(response);
    }
}

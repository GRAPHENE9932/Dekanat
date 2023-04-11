#include "WelcomePageController.hpp"

void WelcomePageController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    auto parameters {request->getParameters()};
    drogon::HttpViewData view_data;
    auto response = drogon::HttpResponse::newHttpViewResponse("WelcomePage.csp", view_data);
    callback(response);
}

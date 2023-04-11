#include "SignUpPageController.hpp"

void SignUpPageController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    auto response = drogon::HttpResponse::newHttpViewResponse("SignUpPage.csp");
    callback(response);
}
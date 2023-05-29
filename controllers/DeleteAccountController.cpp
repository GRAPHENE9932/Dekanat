#include "DeleteAccountController.hpp"
#include "models/Admin.hpp"
#include <drogon/HttpResponse.h>

void DeleteAccountController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    std::optional<Admin> admin = Admin::get_from_session(*request->getSession());
    if (!admin.has_value()) {
        auto response = drogon::HttpResponse::newNotFoundResponse();
        callback(response);
        return;
    }

    admin->remove_from_database();

    request->session()->erase("email");
    request->session()->erase("password");
    
    auto response = drogon::HttpResponse::newRedirectionResponse("/");
    callback(response);
}
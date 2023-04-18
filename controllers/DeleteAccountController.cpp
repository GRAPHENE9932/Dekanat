#include "DeleteAccountController.hpp"
#include "main_database_manager.hpp"
#include <drogon/HttpResponse.h>

void DeleteAccountController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    if (!main_db::validate_admin_session(request->getSession())) {
        auto response = drogon::HttpResponse::newNotFoundResponse();
        callback(response);
        return;
    }

    std::string email = request->session()->get<std::string>("email");
    main_db::delete_admin(email);

    request->session()->erase("email");
    request->session()->erase("password");
    
    auto response = drogon::HttpResponse::newRedirectionResponse("/");
    callback(response);
}
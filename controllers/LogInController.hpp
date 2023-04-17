#pragma once

#include <drogon/HttpController.h>

class LogInController : public drogon::HttpController<LogInController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(LogInController::submit, "/login/submit");
    ADD_METHOD_TO(LogInController::show_page, "/login");
    ADD_METHOD_TO(LogInController::logout, "/logout");
    METHOD_LIST_END

    void submit(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback
    );

    void show_page(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback
    );

    void logout(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback
    );
};
#pragma once

#include <drogon/HttpSimpleController.h>

class LogInPageController : public drogon::HttpSimpleController<LogInPageController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/login");
    PATH_LIST_END
};
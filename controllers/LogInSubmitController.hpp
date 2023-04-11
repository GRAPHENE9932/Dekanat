#pragma once

#include <drogon/HttpSimpleController.h>

class LogInSubmitController : public drogon::HttpSimpleController<LogInSubmitController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/login/submit");
    PATH_LIST_END
};

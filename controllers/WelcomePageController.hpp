#pragma once

#include <drogon/HttpSimpleController.h>

class WelcomePageController : public drogon::HttpSimpleController<WelcomePageController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/");
    PATH_LIST_END
};

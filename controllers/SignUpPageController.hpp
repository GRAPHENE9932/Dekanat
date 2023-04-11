#pragma once

#include <drogon/HttpSimpleController.h>

class SignUpPageController : public drogon::HttpSimpleController<SignUpPageController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/signup");
    PATH_LIST_END
};
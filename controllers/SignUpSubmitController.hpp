#pragma once

#include <drogon/HttpSimpleController.h>

class SignUpSubmitController : public drogon::HttpSimpleController<SignUpSubmitController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/signup/submit");
    PATH_LIST_END
};

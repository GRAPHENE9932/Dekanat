#pragma once

#include <drogon/HttpController.h>

class SignUpController : public drogon::HttpController<SignUpController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SignUpController::submit, "/signup/submit");
    ADD_METHOD_TO(SignUpController::show_page, "/signup");
    METHOD_LIST_END

    void submit(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback
    );

    void show_page(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback
    );
};
#pragma once

#include <drogon/HttpSimpleController.h>

class DeleteAccountController : public drogon::HttpSimpleController<DeleteAccountController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/delete-account");
    PATH_LIST_END
};
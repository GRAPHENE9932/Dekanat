#pragma once

#include <drogon/HttpSimpleController.h>

class GroupPageController : public drogon::HttpSimpleController<GroupPageController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/group");
    PATH_LIST_END
};
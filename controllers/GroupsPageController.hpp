#pragma once

#include <drogon/HttpSimpleController.h>

class GroupsPageController : public drogon::HttpSimpleController<GroupsPageController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/groups");
    PATH_LIST_END
};
#pragma once

#include <drogon/HttpSimpleController.h>

class NewGroupPageController : public drogon::HttpSimpleController<NewGroupPageController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    ) override;
    PATH_LIST_BEGIN
    PATH_ADD("/new-group");
    PATH_LIST_END
};

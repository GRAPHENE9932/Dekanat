#pragma once

#include <drogon/HttpController.h>

class GroupsPageController : public drogon::HttpController<GroupsPageController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(GroupsPageController::delete_group, "/delete-group?name={}");
    ADD_METHOD_TO(GroupsPageController::show_page, "/groups");
    METHOD_LIST_END

    void show_page(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback
    );

    void delete_group(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback,
        const std::string& name
    );
};
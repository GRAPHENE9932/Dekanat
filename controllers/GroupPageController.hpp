#pragma once

#include <drogon/HttpController.h>

class GroupPageController : public drogon::HttpController<GroupPageController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(GroupPageController::show, "/group?name={1}");
    ADD_METHOD_TO(GroupPageController::delete_student, "/delete-student?group={1}&email={2}");
    METHOD_LIST_END

    void show(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback,
        const std::string& name
    );
    void delete_student(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback,
        const std::string& group,
        const std::string& email
    );
};
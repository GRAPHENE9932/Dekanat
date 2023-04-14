#pragma once

#include <drogon/HttpController.h>

class NewStudentPageController : public drogon::HttpController<NewStudentPageController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(NewStudentPageController::submit, "/new-student/submit?group={1}");
    ADD_METHOD_TO(NewStudentPageController::show_page, "/new-student?group={1}");
    METHOD_LIST_END

    void submit(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback,
        const std::string& group
    );

    void show_page(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback,
        const std::string& group
    );
};
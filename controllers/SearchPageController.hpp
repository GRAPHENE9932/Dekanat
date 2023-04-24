#pragma once

#include <drogon/HttpController.h>

class SearchPageController : public drogon::HttpController<SearchPageController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SearchPageController::show_page, "/search?query={}");
    METHOD_LIST_END

    void show_page(
        const drogon::HttpRequestPtr& request,
        std::function<void (const drogon::HttpResponsePtr&)>&& callback,
        const std::string& query
    );
};
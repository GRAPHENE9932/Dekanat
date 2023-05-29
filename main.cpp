#include "random_generator.hpp"

#include <drogon/drogon.h>

int main() {
    seed_randomizer_with_time();
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 8080);
    //Load config file
    drogon::app().loadConfigFile("static/config.json");
    // Enable sessions.
    drogon::app().enableSession();
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}

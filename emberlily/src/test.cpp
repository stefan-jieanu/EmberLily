#include "test.hpp"

void print_int(int i) {
    std::cout << "The number is: " << i << std::endl;
}

void checkglfw() {
    if (glfwInit()) {
        std::cout << "We got g I think\n";
    }
}

void logMe() {
    LOG_TRACE("I am log trace");
    LOG_DEBUG("I am log debug");
    LOG_INFO("I am log info");
    LOG_WARN("I am log warn");
    LOG_ERROR("I am log error");
}
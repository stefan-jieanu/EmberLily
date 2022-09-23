#include "test.hpp"

void print_int(int i) {
    std::cout << "The number is: " << i << std::endl;
}

void checkglfw() {
    if (glfwInit()) {
        std::cout << "We got glfw here too nice I think\n";
    }
}
#include <iostream>
#include <GLFW/glfw3.h>

int main() {
    if (!glfwInit()) {
        std::cout << "Could not init glfw!\n";
    } else {
        std::cout << "Init success!\n";
    }


}
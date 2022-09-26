#include <EmberLily.hpp>

int main()
{
    ember::Logger::Init();
    ember::Window window = ember::Window();

    while (true) {
        window.update();
    }
}
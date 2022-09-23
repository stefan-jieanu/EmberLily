#include <EmberLily.hpp>

int main()
{
    ember::Logger::Init();
    LOG_DEBUG("Client log still works");
    EM_ASSERT(1==0, "you bad at math");
}
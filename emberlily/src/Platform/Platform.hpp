#pragma once

#include <cstdlib>
#include <cstring>

namespace ember
{
    
void* platformAllocate(unsigned long long size, bool aligend)
{
    return malloc(size);
}

void platformFree(void* block, bool aligned)
{
    free(block);
}

void* platformZeroMemory(void* block, unsigned long long size)
{
    return memset(block, 0, size);
}

void* platformSetMemory(void* dest, int value, unsigned long long size)
{
    return memset(dest, value, size);
}

} // namespace ember

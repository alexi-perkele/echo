#include "thread_guard.hpp"

ThreadGuard::ThreadGuard(std::thread t_): t(std::move(t_))
{
    if(!t.joinable())
    {
        perror("thread error");
    }
}

ThreadGuard::~ThreadGuard()
{
    if(t.joinable())
    {
        t.join();
    }
}


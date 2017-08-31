#pragma once

#include<thread>

class ThreadGuard
{
    std::thread t;
public:
    explicit ThreadGuard(std::thread t_);
    ~ThreadGuard();
    // Non copyable:
    
    ThreadGuard(const ThreadGuard& ) = delete;
    ThreadGuard& operator=(const ThreadGuard& ) = delete;
};


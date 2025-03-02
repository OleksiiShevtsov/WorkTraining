#pragma once

#include "common.h"
#include "queue.h"

#include <thread>
#include <memory>
#include <iostream>//delete

class Client
{
public:
    Client(const Client&) = delete;
    Client(std::shared_ptr<Queue> queue, type::ID id, type::Priority priority, const char *data);

    void transferDataToQueue();
    void queueRequest();

private:

    std::shared_ptr<Queue> m_queue;
    type::tagTDATA m_tagtdata;
};

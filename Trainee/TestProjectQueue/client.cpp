#include "client.h"

Client::Client(std::shared_ptr<Queue> queue, type::ID id, type::Priority priority, const char *data) :
    m_tagtdata{id, priority, 0, data},
    m_queue{queue}
{
    queueRequest();
}

void Client::queueRequest(){
    std::thread th(&Client::transferDataToQueue, this);
    th.detach();
}

void Client::transferDataToQueue(){
    while(true) {
        m_tagtdata.ticks = clock();
        m_queue->push(m_tagtdata);
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}

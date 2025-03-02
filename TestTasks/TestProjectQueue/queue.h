#pragma once

#include "common.h"
#include <mutex>
#include <chrono>

class Queue{
public:
    Queue(const Queue&) = delete;
    Queue operator=(const Queue&) = delete;

    Queue();
    ~Queue();

    void push(type::tagTDATA tdata);
    type::tagTDATA pop();

    int size() const;

private:
    struct Node{
        Node(type::tagTDATA tdata);
        type::tagTDATA m_tdata;
        Node* next;
    };
    Node* head;
    int m_size;

    std::mutex m_mutexForReadAndWrite;
};

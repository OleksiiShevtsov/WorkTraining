#include "queue.h"
#include <iostream>//delete

Queue::Queue():
    head{nullptr},
    m_size{0}
{
}

Queue::~Queue(){
    for(Node* it = head; it != nullptr;){
        Node* temp = it->next;
        delete it;
        it = temp;
    }
    m_size = 0;
}

void Queue::push(type::tagTDATA tdata){
    //std::cout << "PUSH: " << std::endl;
    std::lock_guard<std::mutex> guard(m_mutexForReadAndWrite);

    if(head == nullptr){
        head = new Node(tdata);
    }
    else{
        for(Node* it = head; it != nullptr; it = it->next){
            if(tdata.priority <= head->m_tdata.priority){
                Node* newNode = new Node(tdata);
                newNode->next = head;
                head = newNode;
                break;
            }
            else if(tdata.priority >= it->m_tdata.priority && it->next == nullptr){
                it->next = new Node(tdata);
                break;
            }
            else if(tdata.priority >= it->m_tdata.priority && tdata.priority <= it->next->m_tdata.priority){
                Node* newNode = new Node(tdata);
                Node* temp = it->next;
                it->next = newNode;
                newNode->next = temp;
                break;
            }
        }
    }
    m_size++;
}

type::tagTDATA Queue::pop(){
    //std::cout << "POP: " << std::endl;
    std::lock_guard<std::mutex> guard(m_mutexForReadAndWrite);

    if(head == nullptr){
        return type::tagTDATA{0, 0, 0};
    }

    type::tagTDATA returnTdata = head->m_tdata;
    Node* deleteHead = head;
    head = head->next;
    delete deleteHead;
    deleteHead = nullptr;
    m_size--;

    return returnTdata;
}

int Queue::size() const{
    return m_size;
}

Queue::Node::Node(type::tagTDATA tdata) :
    m_tdata{tdata},
    next{nullptr}
{
}

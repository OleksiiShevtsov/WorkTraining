#include "server.h"

Server::Server(std::string pathLogFile, std::shared_ptr<Queue> queue, std::chrono::seconds serverUptime):
    m_pathLogFile{pathLogFile},
    m_queue{queue},
    m_serverUptime{serverUptime}
{
    std::cout << "server is ready to work..." << std::endl;
    try {
        m_fout.open(m_pathLogFile, std::ios::out | std::ios::trunc);
        std::cout << "open file log.txt" << std::endl;
    }
    catch (const std::fstream::failure& e) {
        std::cout << "Exception opening/reading file";
    }
}

Server::~Server(){
    m_fout.close();
    std::cout << "close server..." << std::endl;
}

void Server::start(){
    std::cout << "start server..." << std::endl;
    m_threadServer = std::make_shared<std::thread>(&Server::handleDataInQueue, this);
}

void Server::join(){
    m_threadServer->join();
}

void Server::handleDataInQueue(){
    auto startServer = std::chrono::high_resolution_clock::now();
    while(true){
        auto currentTime  = std::chrono::high_resolution_clock::now();
        if((currentTime - startServer) > m_serverUptime){
            std::cout << "stop server..." <<std::endl;
            return;
        }
        if(m_queue->size() != 0){
            type::tagTDATA newTdata = m_queue->pop();
            newTdata.ticks = clock() - newTdata.ticks;
            writeDataToFile(newTdata);
        }
    }
}

void Server::writeDataToFile(type::tagTDATA tdata){

    m_fout << std::endl;
    m_fout << "id: " << tdata.id << std::endl;
    m_fout << "priority: " << (int)tdata.priority << std::endl;
    m_fout << "ticks: " << tdata.ticks << std::endl;
    m_fout << "data: " << tdata.data << std::endl;
    m_fout << "-------------------------" << std::endl;
}

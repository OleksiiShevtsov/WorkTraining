#pragma once
#include "common.h"
#include "queue.h"
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <memory>

class Server {
public:
    Server(std::string pathLogFile, std::shared_ptr<Queue> queue, std::chrono::seconds serverUptime);
    ~Server();

    void start();
    void join();
    void handleDataInQueue();
    void writeDataToFile(type::tagTDATA tdata);

private:

    std::shared_ptr<std::thread> m_threadServer;
    std::chrono::seconds m_serverUptime;
    std::shared_ptr<Queue> m_queue;
    std::string m_pathLogFile;
    std::ofstream m_fout;
};


#include "client.h"
#include "queue.h"
#include "server.h"

#include <vector>
#include <chrono>

int main()
{
    auto queue = std::make_shared<Queue>();
    Server server("../log.txt", queue, std::chrono::seconds{2});
    server.start();

    std::vector<std::unique_ptr<Client>> clients;
    int numberOfClients = 10;

    for(int i = 1; i <= numberOfClients; i++){
        clients.push_back(std::make_unique<Client>(queue, i, rand()%numberOfClients, "text"));
    }

    server.join();

    return 0;
}


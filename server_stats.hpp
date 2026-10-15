#pragma once

#include <chrono>
#include <cstddef>

class ServerStats
{
public:
    ServerStats();

    void clientConnected();
    void clientDisconnected();

    void messageProcessed(std::size_t bytes);

    std::size_t connectedClients() const;
    std::size_t messagesProcessed() const;
    std::size_t bytesTransferred() const;

    long uptimeSeconds() const;

private:
    std::size_t connectedClients_;
    std::size_t messagesProcessed_;
    std::size_t bytesTransferred_;

    std::chrono::steady_clock::time_point startTime_;
};
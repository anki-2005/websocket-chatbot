#include "dashboard.hpp"

#include <iomanip>
#include <iostream>
#include "logger.hpp"
void Dashboard::display(const ServerStats& stats) const
{
    std::cout << "\033[2J\033[H";

    long uptime = stats.uptimeSeconds();

    long hours = uptime / 3600;
    long minutes = (uptime % 3600) / 60;
    long seconds = uptime % 60;

    std::cout
        << "=========================================\n"
        << "      Real-Time TCP Chat Server\n"
        << "=========================================\n\n";

    std::cout
        << "Status              : RUNNING\n";

    std::cout
        << "Connected Clients   : "
        << stats.connectedClients()
        << '\n';

    std::cout
        << "Messages Processed  : "
        << stats.messagesProcessed()
        << '\n';

    std::cout
        << "Bytes Transferred   : "
        << stats.bytesTransferred()
        << " Bytes\n";

    std::cout
        << "Server Uptime       : "
        << std::setfill('0')
        << std::setw(2) << hours << ":"
        << std::setw(2) << minutes << ":"
        << std::setw(2) << seconds
        << '\n';

    std::cout
        << "\n=========================================\n";
}
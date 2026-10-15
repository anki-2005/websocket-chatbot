#include "config.hpp"

#include <fstream>
#include <sstream>

unsigned short Config::port_ = 8080;
bool Config::load(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file)
    {
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind("PORT=", 0) == 0)
        {
            port_ = static_cast<unsigned short>(
                std::stoi(line.substr(5))
            );
        }
    }

    return true;
}
unsigned short Config::getPort()
{
    return port_;
}
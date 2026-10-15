#pragma once

#include <string>

class Config
{
public:
    static bool load(const std::string& filename);

    static unsigned short getPort();

private:
    static unsigned short port_;
};
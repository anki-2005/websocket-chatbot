#include "logger.hpp"

#include <iostream>

void Logger::log(
    const std::string& level,
    const std::string& message)
{
    std::cout
        << "[" << level << "] "
        << message
        << '\n';
}

void Logger::info(const std::string& message)
{
    log("INFO", message);
}

void Logger::warning(const std::string& message)
{
    log("WARNING", message);
}

void Logger::error(const std::string& message)
{
    log("ERROR", message);
}
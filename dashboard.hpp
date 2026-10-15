#pragma once

#include "server_stats.hpp"

class Dashboard
{
public:
    void display(const ServerStats& stats) const;
};
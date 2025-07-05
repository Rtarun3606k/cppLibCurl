#pragma once
#include <ctime>

struct PricePoint
{
    std::time_t timestamp;
    double price;
};

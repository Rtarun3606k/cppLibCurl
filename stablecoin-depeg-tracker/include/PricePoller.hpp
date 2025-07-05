#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <ctime>
#include "types.hpp"

class PricePoller
{
public:
    PricePoller(const std::string &coinID);
    ~PricePoller();

    void start();
    void stop();

    std::vector<PricePoint> getHistory();

private:
    void fetchLoop();
    double fetchPrice();

    std::string coinId;
    std::thread worker;
    std::atomic<bool> running;
    std::vector<PricePoint> priceHistory;
    std::mutex historyMutex;
};
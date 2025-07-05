#include "PricePoller.hpp"
#include <chrono>
#include <iostream>
#include <curl/curl.h>
#include <json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <cstdio>

using json = nlohmann::json;

PricePoller::PricePoller(const std::string &id) : coinId(id), running(false) {}

PricePoller::~PricePoller()
{
    stop();
}

void PricePoller::start()
{
    running = true;
    worker = std::thread(&PricePoller::fetchLoop, this);
}

void PricePoller::stop()
{
    running = false;
    if (worker.joinable())
        worker.join();
}

std::vector<PricePoint> PricePoller::getHistory()
{
    std::lock_guard<std::mutex> lock(historyMutex);
    return priceHistory;
}

void PricePoller::fetchLoop()
{
    while (running)
    {
        double price = fetchPrice(); // you can stub this for now

        std::lock_guard<std::mutex> lock(historyMutex);
        priceHistory.push_back({std::time(nullptr), price});
        if (priceHistory.size() > 100)
        {
            priceHistory.erase(priceHistory.begin());
        }
        std::cout << "[PricePoller] Latest Price: " << price << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t totalSize = size * nmemb;
    output->append((char *)contents, totalSize);
    return totalSize;
}

double PricePoller::fetchPrice()
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    double price = 1.0;

    std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + coinId + "&vs_currencies=usd";

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (TarunDepegTracker/1.0)");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            if (readBuffer.find("429") != std::string::npos || readBuffer.find("rate limit") != std::string::npos)
            {
                spdlog::warn("[fetchPrice] Rate limited. Backing off...");
                spdlog::debug("{}", readBuffer);
                std::this_thread::sleep_for(std::chrono::seconds(15)); // backoff
                return price;                                          // fallback
            }

            try
            {
                auto j = json::parse(readBuffer);
                if (j.contains(coinId) && j[coinId].contains("usd"))
                {
                    price = j[coinId]["usd"].get<double>();
                }
            }
            catch (const std::exception &e)
            {
                spdlog::error("[fetchPrice] Parse error: {}", e.what());
                spdlog::debug("Raw JSON: {}", readBuffer);
            }
        }
        else
        {
            spdlog::error("[fetchPrice] CURL error: {}", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return price;
}

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/color.h>
#include "PricePoller.hpp"
#include "DepegRiskEngine.hpp"
#include <thread>
#include <chrono>
#include <map>
#include <memory>

int main()
{
    std::vector<std::string> coins = {"tether", "usd-coin", "dai"};
    std::unordered_map<std::string, fmt::color> coinColors = {
        {"tether", fmt::color::green},
        {"usd-coin", fmt::color::blue},
        {"dai", fmt::color::orange}};
    std::map<std::string, std::unique_ptr<PricePoller>> pollers;
    std::map<std::string, DeepegRiskEngine> engines;

    for (const auto &coin : coins)
    {
        pollers[coin] = std::make_unique<PricePoller>(coin);
        pollers[coin]->start();
        engines.emplace(coin, DeepegRiskEngine());
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        for (const auto &coin : coins)
        {
            auto &poller = pollers[coin];
            auto &engine = engines[coin];
            auto history = poller->getHistory();

            auto color = coinColors[coin];
            if (history.size() < 3)
            {
                spdlog::info(fmt::format(fg(color), "[{}] Waiting for more data...", coin));
                continue;
            }

            std::string reason;
            RiskLevel risk = engine.calculateRisk(history, reason);

            switch (risk)
            {
            case RiskLevel::LOW:
                spdlog::info(fmt::format(fg(color), "[{}] Stable | Reason: {}", coin, reason));
                break;
            case RiskLevel::MEDIUM:
                spdlog::warn(fmt::format(fg(color), "[{}] Risk: MEDIUM | {}", coin, reason));
                break;
            case RiskLevel::HIGH:
                spdlog::error(fmt::format(fg(color), "[{}] Risk: HIGH | {}", coin, reason));
                break;
            case RiskLevel::CRITICAL:
                spdlog::critical(fmt::format(fg(color), "[{}] Risk: CRITICAL | {}", coin, reason));
                break;
            }
        }
    }

    return 0;
}

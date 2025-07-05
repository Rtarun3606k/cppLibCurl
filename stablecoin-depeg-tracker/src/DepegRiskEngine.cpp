#include "DepegRiskEngine.hpp"
#include <cmath>

RiskLevel DeepegRiskEngine::calculateRisk(const std::vector<PricePoint> &priceHistory, std::string &reason)
{
    if (priceHistory.empty())
    {
        reason = "No price history available.";
        return RiskLevel::LOW;
    }
    double latest = priceHistory.back().price;
    double sum = 0;
    for (const auto &p : priceHistory)
    {
        sum += p.price;
    }

    double average = sum / priceHistory.size();
    double deviation = std::abs(latest - 1.0);

    double volatility = std::abs(latest - average);

    if (deviation > 0.03 && volatility > 0.02)
    {
        reason = "Severe depeg and volatility";
        return RiskLevel::CRITICAL;
    }
    else if (deviation > 0.02)
    {
        reason = "Major price drop";
        return RiskLevel::HIGH;
    }
    else if (deviation > 0.01)
    {
        reason = "Moderate depeg risk";
        return RiskLevel::MEDIUM;
    }
    else
    {
        reason = "Stable";
        return RiskLevel::LOW;
    }
}

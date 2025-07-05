#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "types.hpp"

enum class RiskLevel
{
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

class DeepegRiskEngine
{
public:
    RiskLevel calculateRisk(const std::vector<PricePoint> &priceHistory, std::string &reason);
};

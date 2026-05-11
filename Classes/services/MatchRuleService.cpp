/**
 * @file MatchRuleService.cpp
 * @brief Implementation of MatchRuleService.
 */

#include "MatchRuleService.h"
#include <cmath>

bool MatchRuleService::canMatch(const std::shared_ptr<CardModel>& desktop, const std::shared_ptr<CardModel>& topStack) {
    if (!desktop || !topStack) return false;

    int face1 = static_cast<int>(desktop->getFace());
    int face2 = static_cast<int>(topStack->getFace());

    return std::abs(face1 - face2) == 1;
}

/**
 * @file MatchRuleService.h
 * @brief Service for checking matching rules.
 */

#ifndef __MATCH_RULE_SERVICE_H__
#define __MATCH_RULE_SERVICE_H__

#include "models/CardModel.h"
#include <memory>

 /**
  * @class MatchRuleService
  * @brief Provides static methods to check game rules.
  */
class MatchRuleService {
public:
    /**
     * @brief Checks if a card from the desktop can be matched with the top stack card.
     * @param desktop The card on the playfield.
     * @param topStack The card on top of the stack.
     * @return true if they match, false otherwise.
     */
    static bool canMatch(const std::shared_ptr<CardModel>& desktop, const std::shared_ptr<CardModel>& topStack);
};

#endif // __MATCH_RULE_SERVICE_H__

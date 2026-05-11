/**
 * @file CardResConfig.h
 * @brief Configuration for card resource paths.
 */

#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include "models/GameTypes.h"
#include <string>

 /**
  * @class CardResConfig
  * @brief Helper to get resource paths for cards.
  */
class CardResConfig {
public:
    static constexpr const char* kCardFrontPrefix = "card_front_";

    /**
     * @brief Gets the texture filename for a card face and suit.
     * @param face The card face.
     * @param suit The card suit.
     * @return The filename string.
     */
    static std::string getFrontTexture(CardFaceType face, CardSuitType suit) {
        return kCardFrontPrefix + std::to_string(static_cast<int>(face)) + "_" + std::to_string(static_cast<int>(suit)) + ".png";
    }
};

#endif // __CARD_RES_CONFIG_H__

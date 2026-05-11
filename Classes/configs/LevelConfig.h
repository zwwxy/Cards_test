/**
 * @file LevelConfig.h
 * @brief Data structures for level configuration.
 */

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "models/GameTypes.h"
#include <vector>
#include <string>

 /**
  * @struct CardConfig
  * @brief Configuration for a single card in a level.
  */
struct CardConfig {
    int cardFace;
    CardSuitType cardSuit;
    cocos2d::Vec2 position;
};

/**
 * @class LevelConfig
 * @brief Configuration for a game level, containing playfield and stack cards.
 */
class LevelConfig {
public:
    std::vector<CardConfig> playfield;
    std::vector<CardConfig> stack;
};

#endif // __LEVEL_CONFIG_H__

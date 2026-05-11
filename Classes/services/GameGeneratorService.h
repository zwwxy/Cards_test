/**
 * @file GameGeneratorService.h
 * @brief Service for generating the initial game state.
 */

#ifndef __GAME_GENERATOR_SERVICE_H__
#define __GAME_GENERATOR_SERVICE_H__

#include "models/GameModel.h"
#include "configs/LevelConfig.h"
#include <memory>

 /**
  * @class GameGeneratorService
  * @brief Generates a GameModel from a LevelConfig.
  */
class GameGeneratorService {
public:
    /**
     * @brief Generates the game model.
     * @param config The level configuration.
     * @return Shared pointer to the generated GameModel.
     */
    static std::shared_ptr<GameModel> generate(const std::shared_ptr<LevelConfig>& config);
};

#endif // __GAME_GENERATOR_SERVICE_H__

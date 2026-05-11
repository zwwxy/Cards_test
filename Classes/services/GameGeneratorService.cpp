/**
 * @file GameGeneratorService.cpp
 * @brief Implementation of GameGeneratorService.
 */

#include "GameGeneratorService.h"

std::shared_ptr<GameModel> GameGeneratorService::generate(const std::shared_ptr<LevelConfig>& config) {
    auto model = std::make_shared<GameModel>();

    if (!config) return model;

    int idCounter = 0;

    // Playfield
    for (const auto& c : config->playfield) {
        auto card = std::make_shared<CardModel>(++idCounter, static_cast<CardFaceType>(c.cardFace), c.cardSuit, c.position, CardState::IN_PLAYFIELD);
        model->addPlayfieldCard(card);
    }

    // Stack
    for (const auto& c : config->stack) {
        auto card = std::make_shared<CardModel>(++idCounter, static_cast<CardFaceType>(c.cardFace), c.cardSuit, c.position, CardState::IN_STACK);
        model->pushToStack(card);
    }

    // Pop one to be the initial top stack card
    if (!model->isStackEmpty()) {
        model->setTopStackCard(model->popFromStack());
    }

    return model;
}

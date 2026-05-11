/**
 * @file GameModel.cpp
 * @brief Implementation of GameModel.
 */

#include "GameModel.h"
#include <algorithm>

void GameModel::addPlayfieldCard(const std::shared_ptr<CardModel>& card) {
    if (card) {
        _playfieldCards.push_back(card);
    }
}

void GameModel::setTopStackCard(const std::shared_ptr<CardModel>& card) {
    moveCardToTop(card);
}

std::shared_ptr<CardModel> GameModel::getTopStackCard() const {
    if (_stackCards.empty()) return nullptr;
    return _stackCards.back();
}

void GameModel::pushToStack(const std::shared_ptr<CardModel>& card) {
    if (card) {
        _stackCards.push_back(card);
    }
}

std::shared_ptr<CardModel> GameModel::popFromStack() {
    if (_stackCards.empty()) return nullptr;
    auto card = _stackCards.back();
    _stackCards.pop_back();
    return card;
}

void GameModel::replaceTopStack(const std::shared_ptr<CardModel>& newCard) {
    if (newCard) {
        _stackCards.push_back(newCard);
    }
}

void GameModel::removePlayfieldCard(const std::shared_ptr<CardModel>& card) {
    auto it = std::find(_playfieldCards.begin(), _playfieldCards.end(), card);
    if (it != _playfieldCards.end()) {
        _playfieldCards.erase(it);
    }
}

std::shared_ptr<CardModel> GameModel::getCardById(int id) const {
    for (const auto& card : _stackCards) {
        if (card->getId() == id) return card;
    }

    for (const auto& card : _playfieldCards) {
        if (card->getId() == id) return card;
    }
    return nullptr;
}

bool GameModel::isStackEmpty() const {
    return _stackCards.empty();
}

void GameModel::removeFromStack(const std::shared_ptr<CardModel>& card) {
    auto it = std::find(_stackCards.begin(), _stackCards.end(), card);
    if (it != _stackCards.end()) {
        _stackCards.erase(it);
    }
}

void GameModel::moveCardToTop(const std::shared_ptr<CardModel>& card) {
    if (!card) return;
    removeFromStack(card);
    pushToStack(card);
}

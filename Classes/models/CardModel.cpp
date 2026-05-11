/**
 * @file CardModel.cpp
 * @brief Implementation of CardModel.
 */

#include "CardModel.h"

CardModel::CardModel(int id, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& pos, CardState state)
    : _id(id), _face(face), _suit(suit), _position(pos), _state(state) {}

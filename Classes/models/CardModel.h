/**
 * @file CardModel.h
 * @brief Data model representing a single card.
 */

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "GameTypes.h"
#include <memory>

 /**
  * @class CardModel
  * @brief Represents a card with face, suit, position, and state.
  */
class CardModel {
public:
    /**
     * @brief Constructor.
     * @param id Unique ID of the card. 每张牌的唯一标识号
     * @param face Face value of the card.  牌面值
     * @param suit Suit of the card.    牌的花色
     * @param pos Initial position of the card. 牌的初始位置
     * @param state Initial state of the card.  牌的初始状态
     */
    CardModel(int id, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& pos, CardState state);
    ~CardModel() = default;

    /**
     * @brief Gets the card ID.
     * @return The card ID.
     */
    int getId() const { return _id; }

    /**
     * @brief Gets the card face value.
     * @return The card face type.
     */
    CardFaceType getFace() const { return _face; }

    /**
     * @brief Gets the card suit.
     * @return The card suit type.
     */
    CardSuitType getSuit() const { return _suit; }

    /**
     * @brief Sets the position of the card.
     * @param pos The new position.
     */
    void setPosition(const cocos2d::Vec2& pos) { _position = pos; }

    /**
     * @brief Gets the current position of the card.
     * @return The position vector.
     */
    cocos2d::Vec2 getPosition() const { return _position; }

    /**
     * @brief Sets the state of the card (e.g., in stack or playfield).
     * @param state The new state.
     */
    void setState(CardState state) { _state = state; }

    /**
     * @brief Gets the current state of the card.
     * @return The card state.
     */
    CardState getState() const { return _state; }

private:
    int _id;
    CardFaceType _face;
    CardSuitType _suit;
    cocos2d::Vec2 _position;
    CardState _state;
};

#endif // __CARD_MODEL_H__

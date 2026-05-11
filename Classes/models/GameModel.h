/**
 * @file GameModel.h
 * @brief Data model representing the entire game state.
 */

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <stack>
#include <memory>

 /**
  * @class GameModel
  * @brief Holds the state of the game, including playfield cards and stack cards.
  * @param _playfieldCards Stores all cards currently on the playfield. 存储当前在牌桌（游戏区域）上的所有卡片。
  * @param _topStackCard Stores the card currently at the top of the stack (visible). 存储当前牌堆顶部的卡片（可见的那张）。
  * @param _stackCards Stores the remaining cards in the stack (invisible). 存储牌堆中剩余的卡片（不可见部分）。
  */
class GameModel {
public:
    GameModel() = default;
    ~GameModel() = default;

    /**
     * @brief Adds a card to the playfield.
     * @param card The card to add.
     */
    void addPlayfieldCard(const std::shared_ptr<CardModel>& card);

    /**
     * @brief Gets the list of cards currently on the playfield.
     * @return Vector of shared pointers to CardModel.
     */
    const std::vector<std::shared_ptr<CardModel>>& getPlayfieldCards() const { return _playfieldCards; }

    /**
     * @brief Gets the list of cards currently in the stack.
     * @return Vector of shared pointers to CardModel.
     */
    const std::vector<std::shared_ptr<CardModel>>& getStackCards() const { return _stackCards; }

    /**
     * @brief Sets the card at the top of the stack.
     * @param card The card to set as top.
     */
    void setTopStackCard(const std::shared_ptr<CardModel>& card);

    /**
     * @brief Gets the card currently at the top of the stack.
     * @return Shared pointer to the top stack card.
     */
    std::shared_ptr<CardModel> getTopStackCard() const;

    /**
     * @brief Pushes a card onto the stack.
     * @param card The card to push.
     */
    void pushToStack(const std::shared_ptr<CardModel>& card);

    /**
     * @brief Pops a card from the stack.
     * @return Shared pointer to the popped card.
     */
    std::shared_ptr<CardModel> popFromStack();

    /**
     * @brief Checks if the stack is empty.
     * @return true if empty, false otherwise.
     */
    bool isStackEmpty() const;

    /**
     * @brief Replaces the top stack card with a new one, pushing the old one down.
     * @param newCard The new top card.
     */
    void replaceTopStack(const std::shared_ptr<CardModel>& newCard);

    /**
     * @brief Removes a card from the playfield.
     * @param card The card to remove.
     */
    void removePlayfieldCard(const std::shared_ptr<CardModel>& card);

    /**
     * @brief Removes a specific card from the stack (used for undo/move).
     * @param card The card to remove.
     */
    void removeFromStack(const std::shared_ptr<CardModel>& card);

    /**
     * @brief Moves a specific card to the top of the stack.
     * @param card The card to move.
     */
    void moveCardToTop(const std::shared_ptr<CardModel>& card);

    /**
     * @brief Finds a card by its ID.
     * @param id The ID of the card.
     * @return Shared pointer to the card, or nullptr if not found.
     */
    std::shared_ptr<CardModel> getCardById(int id) const;

    /**
     * @brief Alias for getCardById.
     * @param id The ID of the card.
     * @return Shared pointer to the card.
     */
    std::shared_ptr<CardModel> getCard(int id) const { return getCardById(id); }

    /**
     * @brief Alias for getTopStackCard.
     * @return Shared pointer to the top stack card.
     */
    std::shared_ptr<CardModel> getStackTop() const { return getTopStackCard(); }

private:
    std::vector<std::shared_ptr<CardModel>> _playfieldCards;
    std::vector<std::shared_ptr<CardModel>> _stackCards;
};

#endif // __GAME_MODEL_H__

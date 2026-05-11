/**
 * @file UndoModel.h
 * @brief Data structure for recording undo operations.
 */

#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <memory>

 /**
  * @enum OperationType
  * @brief Types of operations that can be undone.
  */
enum class OperationType {
    MOVE_WITHIN_STACK,
    MOVE_FROM_PLAYFIELD_TO_STACK
};

/**
 * @struct UndoModel
 * @brief Stores details of a single operation for undo purposes.
 */
struct UndoModel {
    OperationType type;
    int cardId;
    cocos2d::Vec2 fromPos;
    cocos2d::Vec2 toPos;
    CardState prevState;
    int prevTopCardId; // For stack move
    int zOrder;

    UndoModel(OperationType t, int id, cocos2d::Vec2 from, cocos2d::Vec2 to, CardState state, int prevTop = -1, int z = 0)
        : type(t), cardId(id), fromPos(from), toPos(to), prevState(state), prevTopCardId(prevTop), zOrder(z) {}
};

// Alias for compatibility if needed, but we should use UndoModel struct directly
using UndoRecord = UndoModel;

#endif // __UNDO_MODEL_H__

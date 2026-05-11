/**
 * @file UndoManager.cpp
 * @brief 撤销管理器实现文件
 */

#include "UndoManager.h"

 /**
  * @brief 构造函数
  * @param gameModel 游戏数据模型指针
  */
UndoManager::UndoManager(const std::shared_ptr<GameModel>& gameModel)
    : _gameModel(gameModel) {
}

/**
 * @brief 记录一次操作
 *
 * 将操作记录压入撤销栈中。
 *
 * @param rec 撤销记录对象
 */
void UndoManager::record(const UndoRecord& rec) {
    _undoStack.push(rec);
}

/**
 * @brief 执行撤销操作
 *
 * 从栈顶取出一顶记录，根据操作类型恢复模型状态，并触发回调通知视图更新。
 *
 * @return true 撤销成功，false 栈为空或失败
 */
bool UndoManager::undo() {
    if (_undoStack.empty()) return false;

    auto rec = _undoStack.top();
    _undoStack.pop();

    // Restore model state
    // We need to reverse the operation based on type

    if (rec.type == OperationType::MOVE_FROM_PLAYFIELD_TO_STACK) {
        // Reverse: Move from Stack back to Playfield
        auto card = _gameModel->getCardById(rec.cardId);
        if (card) {
            _gameModel->popFromStack(); // Remove from stack top
            _gameModel->addPlayfieldCard(card);
            card->setPosition(rec.fromPos);
            card->setState(rec.prevState);
        }
    }
    else if (rec.type == OperationType::MOVE_WITHIN_STACK) {
        // Reverse: Restore previous top card
        auto card = _gameModel->getCardById(rec.cardId);
        if (card) {
            // If we just moved to top, we might need to pop it and push the old one?
            // Or just set the old one as top?
            // In our simplified model, we just need to ensure the previous top is now top.
            auto prevTop = _gameModel->getCardById(rec.prevTopCardId);
            if (prevTop) {
                _gameModel->moveCardToTop(prevTop);
            }
            // And restore position if needed (though stack usually has fixed pos)
            card->setPosition(rec.fromPos);
        }
    }

    if (_undoCallback) {
        _undoCallback(true, rec);
    }

    return true;
}

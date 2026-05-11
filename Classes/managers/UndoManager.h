/**
 * @file UndoManager.h
 * @brief Manages the undo history and operations.
 */

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/GameModel.h"
#include "models/UndoModel.h"
#include <stack>
#include <memory>
#include <functional>

 // UndoRecord is defined in UndoModel.h
 // using UndoRecord = UndoModel; // This was causing the error because UndoModel is not a class but a header name, and UndoRecord is struct in it.

 /**
  * @class UndoManager
  * @brief Handles recording and executing undo operations.
  */
class UndoManager {
public:
    /**
     * @brief Constructor.
     * @param gameModel Shared pointer to the game model.
     */
    UndoManager(const std::shared_ptr<GameModel>& gameModel);

    /**
     * @brief Records an operation for undo.
     * @param rec The undo record.
     */
    void record(const UndoRecord& rec);

    /**
     * @brief Performs the undo operation.
     * @return true if undo was successful, false otherwise.
     */
    bool undo();

    using UndoCallback = std::function<void(bool, const UndoRecord&)>;
    /**
     * @brief Sets the callback to be called after undo.
     * @param cb The callback function.
     */
    void setUndoCallback(UndoCallback cb) { _undoCallback = cb; }

private:
    std::shared_ptr<GameModel> _gameModel;
    std::stack<UndoRecord> _undoStack;
    UndoCallback _undoCallback;
};

#endif // __UNDO_MANAGER_H__

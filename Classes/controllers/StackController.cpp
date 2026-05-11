/**
 * @file StackController.cpp
 * @brief 堆牌区控制器实现文件
 */

#include "StackController.h"
#include "models/UndoModel.h"

USING_NS_CC;

/**
 * @brief 创建 StackController 实例
 *
 * 使用 new (std::nothrow) 分配内存，防止异常。
 * 调用 init 进行初始化，如果失败则释放内存。
 *
 * @param model 游戏数据模型
 * @param view 游戏视图
 * @param undoManager 撤销管理器
 * @return StackController* 创建成功的实例指针
 */
StackController* StackController::create(GameModel* model, GameView* view, UndoManager* undoManager) {
    StackController* controller = new (std::nothrow) StackController();
    if (controller && controller->init(model, view, undoManager)) {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

/**
 * @brief 初始化控制器
 *
 * 保存模型、视图和撤销管理器的引用。
 *
 * @param model 游戏数据模型
 * @param view 游戏视图
 * @param undoManager 撤销管理器
 * @return true 初始化成功，false 失败
 */
bool StackController::init(GameModel* model, GameView* view, UndoManager* undoManager) {
    _gameModel = model;
    _gameView = view;
    _undoManager = undoManager;
    return true;
}

/**
 * @brief 处理堆牌区卡牌点击
 *
 * 如果点击的卡牌在堆牌区且不是顶牌，将其移动到堆顶，并记录撤销操作。
 *
 * @param cardId 被点击卡牌的 ID
 */
void StackController::handleCardClick(int cardId) {
    auto clickedCard = _gameModel->getCard(cardId);
    if (!clickedCard || clickedCard->getState() != CardState::IN_STACK) return;

    auto topCard = _gameModel->getStackTop();
    if (clickedCard == topCard) return; // Already on top

    // Requirement 1: Move clicked stack card to top

    UndoModel undoOp(OperationType::MOVE_WITHIN_STACK,
        clickedCard->getId(),
        clickedCard->getPosition(),
        topCard->getPosition(), // It takes the position of the top card visually?
        CardState::IN_STACK,
        topCard->getId());
    _undoManager->record(undoOp);

    // Update Model
    _gameModel->moveCardToTop(clickedCard);

    // Update View
    auto cardView = _gameView->getCardViewById(cardId);
    if (cardView) {
        // Bring to front
        cardView->setLocalZOrder(100); // Simplified Z-order
        _gameView->playMatchAnimation(cardId, topCard->getPosition());
    }
}

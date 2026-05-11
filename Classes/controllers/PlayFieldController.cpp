/**
 * @file PlayFieldController.cpp
 * @brief 主牌区控制器实现文件
 */

#include "PlayFieldController.h"
#include "services/MatchRuleService.h"
#include "models/UndoModel.h"

USING_NS_CC;

/**
 * @brief 创建 PlayFieldController 实例
 *
 * 使用 new (std::nothrow) 分配内存，防止异常。
 * 调用 init 进行初始化，如果失败则释放内存。
 *
 * @param model 游戏数据模型
 * @param view 游戏视图
 * @param undoManager 撤销管理器
 * @return PlayFieldController* 创建成功的实例指针
 */
PlayFieldController* PlayFieldController::create(GameModel* model, GameView* view, UndoManager* undoManager) {
    PlayFieldController* controller = new (std::nothrow) PlayFieldController();
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
bool PlayFieldController::init(GameModel* model, GameView* view, UndoManager* undoManager) {
    _gameModel = model;
    _gameView = view;
    _undoManager = undoManager;
    return true;
}

/**
 * @brief 处理主牌区卡牌点击
 *
 * 检查点击的卡牌是否能与堆牌区顶牌匹配。如果匹配成功，则执行消除动画并更新数据。
 *
 * @param cardId 被点击卡牌的 ID
 */
void PlayFieldController::handleCardClick(int cardId) {
    auto clickedCard = _gameModel->getCard(cardId);
    if (!clickedCard || clickedCard->getState() != CardState::IN_PLAYFIELD) return;

    auto topStackCard = _gameModel->getStackTop();
    if (!topStackCard) return; // Should not happen if stack initialized

    // Requirement 2: Match with top stack card
    if (MatchRuleService::canMatch(clickedCard, topStackCard)) {
        // Match!

        // 1. Record Undo
        UndoModel undoOp(OperationType::MOVE_FROM_PLAYFIELD_TO_STACK,
            clickedCard->getId(),
            clickedCard->getPosition(),
            topStackCard->getPosition(),
            CardState::IN_PLAYFIELD);
        _undoManager->record(undoOp);

        // 2. Update Model
        _gameModel->pushToStack(clickedCard);
        clickedCard->setState(CardState::IN_STACK);

        // 3. Update View
        auto cardView = _gameView->getCardViewById(cardId);
        if (cardView) {
            _gameView->playMatchAnimation(cardId, topStackCard->getPosition());
        }
    }
}

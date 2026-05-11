/**
 * @file GameController.cpp
 * @brief 游戏控制器实现文件
 */

#include "GameController.h"
#include "configs/LevelConfigLoader.h"
#include "services/GameGeneratorService.h"
#include "services/MatchRuleService.h"
#include "utils/ResolutionUtils.h"

 /**
  * @brief 构造函数
  *
  * 初始化成员变量，将 _gameView 置为空。
  */
GameController::GameController() : _gameView(nullptr) {
}

/**
 * @brief 析构函数
 */
GameController::~GameController() {
    if (_gameView) {
        _gameView->release();
        _gameView = nullptr;
    }
}

/**
 * @brief 开始游戏
 *
 * 加载关卡配置，生成游戏模型，创建视图，设置事件回调，
 * 初始化 UI 界面和撤销管理器。
 *
 * @param levelId 关卡 ID
 */
void GameController::startGame(int levelId) {
    auto config = LevelConfigLoader::loadLevelConfig(levelId);
    _gameModel = GameGeneratorService::generate(config);

    _gameView = GameView::create();
    _gameView->retain();

    _gameView->setCardClickCallback([this](int cardId) {
        _handleCardClick(cardId);
        });

    _gameView->setUndoClickCallback([this]() {
        _handleUndoClick();
        });

    _gameView->initUI(_gameModel);

    _undoManager = std::make_unique<UndoManager>(_gameModel);
    _undoManager->setUndoCallback([this](bool success, const UndoRecord& rec) {
        if (success) {
            if (rec.type == OperationType::MOVE_FROM_PLAYFIELD_TO_STACK) {
                // 播放回退动画：从堆牌区位置(toPos)移动回主牌区位置(fromPos)
                _gameView->playReverseMove(rec.cardId, rec.toPos, rec.fromPos);

                // 恢复卡牌的 Z-Order
                auto cv = _gameView->getCardViewById(rec.cardId);
                if (cv) {
                    cv->setLocalZOrder(rec.zOrder);
                }
            }
            else if (rec.type == OperationType::MOVE_WITHIN_STACK) {
                // 堆牌区内部移动回退
                _gameView->playReverseMove(rec.cardId, rec.toPos, rec.fromPos);

                // 恢复 Z-Order: 之前的顶牌(prevTopCardId)应该在最上面
                auto prevTopCv = _gameView->getCardViewById(rec.prevTopCardId);
                if (prevTopCv) {
                    prevTopCv->setLocalZOrder(100);
                }
                auto cv = _gameView->getCardViewById(rec.cardId);
                if (cv) {
                    cv->setLocalZOrder(0); // 被回退的卡牌放到底层
                }
            }
            else {
                // 其他类型的撤销，直接刷新 UI
                _gameView->initUI(_gameModel);
            }
        }
        });
}

/**
 * @brief 处理卡牌点击事件
 *
 * 检查卡牌是否被遮挡，然后根据卡牌所在区域（主牌区或堆牌区）
 * 分发到相应的处理逻辑。
 *
 * @param cardId 被点击卡牌的 ID
 */
void GameController::_handleCardClick(int cardId) {
    auto card = _gameModel->getCardById(cardId);
    if (!card) return;

    // Check if the card is blocked by other cards
    if (_isCardBlocked(card)) {
        cocos2d::log("Card %d is blocked", cardId);
        return;
    }

    auto topStack = _gameModel->getTopStackCard();

    bool isPlayfield = false;
    for (const auto& c : _gameModel->getPlayfieldCards()) {
        if (c->getId() == cardId) {
            isPlayfield = true;
            break;
        }
    }

    if (isPlayfield) {
        _handlePlayfieldClick(card, topStack);
    }
    else {
        // Check if it is in stack
        bool isStack = false;
        for (const auto& c : _gameModel->getStackCards()) {
            if (c->getId() == cardId) {
                isStack = true;
                break;
            }
        }

        if (isStack) {
            _handleStackClick(card, topStack);
        }
    }
}

/**
 * @brief 处理撤销按钮点击
 *
 * 调用撤销管理器执行撤销操作。
 */
void GameController::_handleUndoClick() {
    _undoManager->undo();
}

/**
 * @brief 检查卡牌是否被遮挡
 *
 * 依次检查主牌区和堆牌区的遮挡情况。
 *
 * @param card 待检查的卡牌
 * @return true 如果被遮挡，false 否则
 */
bool GameController::_isCardBlocked(const std::shared_ptr<CardModel>& card) {
    if (_checkPlayfieldBlockage(card)) return true;
    if (_checkStackBlockage(card)) return true;
    return false;
}

/**
 * @brief 处理主牌区点击逻辑
 *
 * 检查是否可以消除，如果可以则记录撤销、更新模型并播放动画。
 *
 * @param card 被点击的卡牌
 * @param topStack 当前堆牌区顶部的卡牌
 */
void GameController::_handlePlayfieldClick(const std::shared_ptr<CardModel>& card, const std::shared_ptr<CardModel>& topStack) {
    if (MatchRuleService::canMatch(card, topStack)) {
        int zOrder = 0;
        auto cv = _gameView->getCardViewById(card->getId());
        if (cv) zOrder = cv->getLocalZOrder();

        cocos2d::Vec2 targetPos = topStack ? topStack->getPosition() : cocos2d::Vec2(ResolutionUtils::kStackWidth / 2, ResolutionUtils::kStackHeight / 2);

        UndoRecord rec(OperationType::MOVE_FROM_PLAYFIELD_TO_STACK,
            card->getId(),
            card->getPosition(),
            targetPos,
            CardState::IN_PLAYFIELD,
            -1,
            zOrder);

        _undoManager->record(rec);

        _gameModel->removePlayfieldCard(card);
        _gameModel->pushToStack(card); // Use pushToStack instead of replaceTopStack for consistency
        card->setState(CardState::IN_STACK);
        card->setPosition(targetPos);

        _gameView->playMatchAnimation(card->getId(), targetPos);
    }
}

/**
 * @brief 处理堆牌区点击逻辑
 *
 * 如果点击的不是顶牌，则将其移至堆顶，记录撤销并播放动画。
 *
 * @param card 被点击的卡牌
 * @param topStack 当前堆牌区顶部的卡牌
 */
void GameController::_handleStackClick(const std::shared_ptr<CardModel>& card, const std::shared_ptr<CardModel>& topStack) {
    // If it's in stack and not the top one, move it to top
    if (topStack && topStack->getId() != card->getId()) {
        cocos2d::Vec2 targetPos = topStack->getPosition();

        UndoRecord rec(OperationType::MOVE_WITHIN_STACK,
            card->getId(),
            card->getPosition(),
            targetPos,
            CardState::IN_STACK,
            topStack->getId());
        _undoManager->record(rec);

        _gameModel->moveCardToTop(card);
        card->setPosition(targetPos);

        _gameView->playMatchAnimation(card->getId(), targetPos);
    }
}

/**
 * @brief 通用卡牌遮挡检查
 *
 * 遍历给定区域的卡牌列表，检查指定卡牌是否被后面的卡牌遮挡。
 *
 * @param card 待检查卡牌
 * @param cardList 区域内的卡牌列表（主牌区或堆牌区）
 * @param debugPrefix 日志前缀，用于打印调试信息
 * @return true 如果被遮挡，false 否则
 */
bool GameController::_checkBlockageInList(
    const std::shared_ptr<CardModel>& card,
    const std::vector<std::shared_ptr<CardModel>>& cardList,
    const std::string& debugPrefix)
{
    int cardIndex = -1;
    for (size_t i = 0; i < cardList.size(); ++i) {
        if (cardList[i]->getId() == card->getId()) {
            cardIndex = static_cast<int>(i);
            break;
        }
    }

    if (cardIndex == -1) return false;

    auto cardView = _gameView->getCardViewById(card->getId());
    if (!cardView) return false;

    cocos2d::Rect cardRect = cardView->getBoundingBox();
    if (!debugPrefix.empty()) {
        cocos2d::log("%s Checking block for card %d (Index %d). Rect: %.1f, %.1f, %.1f, %.1f",
            debugPrefix.c_str(), card->getId(), cardIndex,
            cardRect.origin.x, cardRect.origin.y,
            cardRect.size.width, cardRect.size.height);
    }

    // 检查该卡牌之后的卡牌是否遮挡
    for (size_t i = cardIndex + 1; i < cardList.size(); ++i) {
        auto otherCard = cardList[i];
        auto otherView = _gameView->getCardViewById(otherCard->getId());
        if (!otherView) continue;

        cocos2d::Rect otherRect = otherView->getBoundingBox();
        if (!debugPrefix.empty()) {
            cocos2d::log("%s  Comparing with card %d (Index %d). Rect: %.1f, %.1f, %.1f, %.1f",
                debugPrefix.c_str(), otherCard->getId(), (int)i,
                otherRect.origin.x, otherRect.origin.y,
                otherRect.size.width, otherRect.size.height);
        }

        if (cardRect.intersectsRect(otherRect)) {
            if (!debugPrefix.empty()) {
                cocos2d::log("%s  -> BLOCKED by card %d", debugPrefix.c_str(), otherCard->getId());
            }
            return true; // 被遮挡
        }
    }
    return false; // 没有遮挡
}

bool GameController::_checkPlayfieldBlockage(const std::shared_ptr<CardModel>& card) {
    return _checkBlockageInList(card, _gameModel->getPlayfieldCards(), "Playfield");
}

bool GameController::_checkStackBlockage(const std::shared_ptr<CardModel>& card) {
    return _checkBlockageInList(card, _gameModel->getStackCards(), "Stack");
}
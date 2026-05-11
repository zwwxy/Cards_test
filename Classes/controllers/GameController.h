/**
 * @file GameController.h
 * @brief Main controller for the game logic.
 */

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"
#include <memory>

 /**
  * @class GameController
  * @brief 游戏核心控制器类
  *
  * 功能描述：
  * 负责整个游戏的核心逻辑控制，包括游戏的初始化、开始、结束以及游戏过程中的状态管理。
  * 它是连接 Model（数据模型）和 View（视图界面）的桥梁。
  *
  * 职责：
  * 1. 初始化游戏环境，加载关卡配置。
  * 2. 创建并管理 GameModel 和 GameView。
  * 3. 处理来自 View 层的用户输入（如点击卡牌、点击撤销）。
  * 4. 调用 Service 层（如 MatchRuleService）进行规则判定。
  * 5. 更新 Model 状态并通知 View 进行相应的 UI 刷新或动画播放。
  * 6. 管理撤销（Undo）操作。
  *
  * 使用场景：
  * 在 AppDelegate 或场景入口处创建，调用 startGame() 启动游戏。
  */
class GameController {
public:
    /**
     * @brief 构造函数
     */
    GameController();

    /**
     * @brief 析构函数
     */
    ~GameController();

    /**
     * @brief 开始指定关卡的游戏
     *
     * 加载关卡配置，生成游戏模型，初始化视图，并设置事件回调。
     *
     * @param levelId 关卡 ID
     */
    void startGame(int levelId);

    /**
     * @brief 获取游戏视图层
     * @return GameView* 游戏视图层的指针
     */
    cocos2d::Layer* getGameViewLayer() const { return _gameView; }

private:
    /**
     * @brief 处理卡牌点击事件
     * @param cardId 被点击卡牌的 ID
     */
    void _handleCardClick(int cardId);

    /**
     * @brief 处理撤销按钮点击事件
     */
    void _handleUndoClick();

    /**
     * @brief 检查卡牌是否被其他卡牌遮挡
     * @param card 待检查的卡牌模型
     * @return true 被遮挡，false 未被遮挡
     */
    bool _isCardBlocked(const std::shared_ptr<CardModel>& card);

    /**
     * @brief 检查主牌区卡牌的遮挡情况
     * @param card 待检查的卡牌
     * @return true 如果被遮挡，false 否则
     */
    bool _checkPlayfieldBlockage(const std::shared_ptr<CardModel>& card);

    /**
     * @brief 检查堆牌区卡牌的遮挡情况
     * @param card 待检查的卡牌
     * @return true 如果被遮挡，false 否则
     */
    bool _checkStackBlockage(const std::shared_ptr<CardModel>& card);

    /**
     * @brief 处理主牌区的点击逻辑
     * @param card 被点击的卡牌
     * @param topStack 当前堆牌区顶部的卡牌
     */
    void _handlePlayfieldClick(const std::shared_ptr<CardModel>& card, const std::shared_ptr<CardModel>& topStack);

    /**
     * @brief 处理堆牌区的点击逻辑
     * @param card 被点击的卡牌
     * @param topStack 当前堆牌区顶部的卡牌
     */
    void _handleStackClick(const std::shared_ptr<CardModel>& card, const std::shared_ptr<CardModel>& topStack);

    bool _checkBlockageInList(
        const std::shared_ptr<CardModel>& card,
        const std::vector<std::shared_ptr<CardModel>>& cardList,
        const std::string& debugPrefix);

    std::shared_ptr<GameModel> _gameModel;      ///< 游戏数据模型
    GameView* _gameView;                        ///< 游戏视图层
    std::unique_ptr<UndoManager> _undoManager;  ///< 撤销管理器
};

#endif // __GAME_CONTROLLER_H__

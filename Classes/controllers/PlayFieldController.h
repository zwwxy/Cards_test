/**
 * @file PlayFieldController.h
 * @brief Controller for handling interactions in the playfield area.
 */

#ifndef __PLAY_FIELD_CONTROLLER_H__
#define __PLAY_FIELD_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"

 /**
  * @class PlayFieldController
  * @brief 主牌区控制器类
  *
  * 功能描述：
  * 专门负责处理主牌区（Playfield）的交互逻辑。
  *
  * 职责：
  * 1. 处理主牌区卡牌的点击事件。
  * 2. 判定卡牌是否可以与堆牌区顶牌消除（Match）。
  * 3. 执行消除逻辑并记录撤销操作。
  *
  * 使用场景：
  * 由 GameController 创建并持有，当检测到点击事件发生在主牌区时调用。
  */
class PlayFieldController : public cocos2d::Ref {
public:
    /**
     * @brief 创建 PlayFieldController 实例
     * @param model 游戏数据模型指针
     * @param view 游戏视图指针
     * @param undoManager 撤销管理器指针
     * @return PlayFieldController* 创建成功的实例指针
     */
    static PlayFieldController* create(GameModel* model, GameView* view, UndoManager* undoManager);

    /**
     * @brief 处理主牌区卡牌点击
     *
     * 检查点击的卡牌是否能与堆牌区顶牌匹配。如果匹配成功，则执行消除动画并更新数据。
     *
     * @param cardId 被点击卡牌的 ID
     */
    void handleCardClick(int cardId);

protected:
    /**
     * @brief 初始化控制器
     * @param model 游戏数据模型
     * @param view 游戏视图
     * @param undoManager 撤销管理器
     * @return true 初始化成功，false 失败
     */
    bool init(GameModel* model, GameView* view, UndoManager* undoManager);

private:
    GameModel* _gameModel;      ///< 游戏数据模型
    GameView* _gameView;        ///< 游戏视图
    UndoManager* _undoManager;  ///< 撤销管理器
};

#endif // __PLAY_FIELD_CONTROLLER_H__

/**
 * @file StackController.h
 * @brief Controller for handling interactions in the stack area.
 */

#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"

 /**
  * @class StackController
  * @brief 堆牌区控制器类
  *
  * 功能描述：
  * 专门负责处理堆牌区（Stack）的交互逻辑。
  *
  * 职责：
  * 1. 处理堆牌区卡牌的点击事件。
  * 2. 执行堆牌区内部的卡牌移动逻辑（如将某张牌移至堆顶）。
  * 3. 记录相关的撤销操作。
  *
  * 使用场景：
  * 由 GameController 创建并持有，当检测到点击事件发生在堆牌区时调用。
  */
class StackController : public cocos2d::Ref {
public:
    /**
     * @brief 创建 StackController 实例
     * @param model 游戏数据模型指针
     * @param view 游戏视图指针
     * @param undoManager 撤销管理器指针
     * @return StackController* 创建成功的实例指针
     */
    static StackController* create(GameModel* model, GameView* view, UndoManager* undoManager);

    /**
     * @brief 处理堆牌区卡牌点击
     *
     * 如果点击的卡牌在堆牌区且不是顶牌，将其移动到堆顶。
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

#endif // __STACK_CONTROLLER_H__

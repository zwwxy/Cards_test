/**
 * @file GameView.h
 * @brief Main game layer handling UI and rendering.
 */

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "CardView.h"
#include <memory>
#include <functional>
#include <vector>

 /**
  * @class GameView
  * @brief 游戏主视图层
  *
  * 功能描述：
  * 该类继承自 cocos2d::Layer，是游戏的主要显示层。
  * 它负责渲染游戏场景，包括所有的扑克牌和 UI 控件（如撤销按钮）。
  * 同时负责接收用户的输入事件，并将其转发给控制器或模型进行处理。
  *
  * 职责：
  * 1. 根据 GameModel 初始化和更新游戏界面。
  * 2. 管理所有的 CardView 对象。
  * 3. 提供卡牌移动、匹配等动画效果的接口。
  * 4. 处理 UI 交互（点击卡牌、点击按钮）。
  *
  * 使用场景：
  * 作为游戏场景的主要子节点，用于展示游戏的核心玩法界面。
  */
class GameView : public cocos2d::Layer {
public:
    /**
     * @brief 创建 GameView 实例
     *
     * 静态工厂方法，用于创建并初始化一个 GameView 对象。
     *
     * @return GameView* 返回创建成功的 GameView 指针，如果创建失败则返回 nullptr。
     */
    static GameView* create();

    /**
     * @brief 初始化游戏 UI
     *
     * 根据提供的游戏模型数据，创建并显示所有的扑克牌视图。
     * 会先清除现有的所有卡牌视图，然后重新生成。
     *
     * @param model 指向 GameModel 的共享指针，包含当前游戏的完整状态。
     */
    void initUI(const std::shared_ptr<GameModel>& model);

    /**
     * @brief 设置卡牌点击回调
     *
     * 当任意一张扑克牌被点击时，会调用此回调函数。
     *
     * @param cb 回调函数，参数为被点击扑克牌的 ID (int)。
     */
    void setCardClickCallback(const std::function<void(int)>& cb) { _cardClickCallback = cb; }

    /**
     * @brief 设置撤销按钮点击回调
     *
     * 当撤销按钮被点击时，会调用此回调函数。
     *
     * @param cb 回调函数，无参数。
     */
    void setUndoClickCallback(const std::function<void()>& cb) { _undoClickCallback = cb; }

    /**
     * @brief 播放卡牌匹配动画
     *
     * 将指定 ID 的卡牌移动到目标位置，通常用于消除或移动到堆叠区的动画。
     *
     * @param cardId 需要移动的卡牌 ID。
     * @param toPos 移动的目标位置 (cocos2d::Vec2)。
     */
    void playMatchAnimation(int cardId, const cocos2d::Vec2& toPos);

    /**
     * @brief 播放撤销移动动画
     *
     * 将指定 ID 的卡牌从起始位置移动到目标位置，用于表现撤销操作。
     *
     * @param cardId 需要移动的卡牌 ID。
     * @param fromPos 动画起始位置 (cocos2d::Vec2)。
     * @param toPos 动画结束位置 (cocos2d::Vec2)。
     */
    void playReverseMove(int cardId, const cocos2d::Vec2& fromPos, const cocos2d::Vec2& toPos);

    /**
     * @brief 获取卡牌视图对象
     *
     * 根据卡牌 ID 查找对应的 CardView 对象。
     *
     * @param cardId 要查找的卡牌 ID。
     * @return CardView* 找到的 CardView 指针，如果未找到则返回 nullptr。
     */
    CardView* getCardViewById(int cardId);

    /**
     * @brief 添加卡牌视图
     *
     * 根据 CardModel 创建一个新的 CardView 并添加到场景中。
     *
     * @param card 指向 CardModel 的共享指针。
     */
    void addCardView(const std::shared_ptr<CardModel>& card);

protected:
    /**
     * @brief 初始化层
     *
     * 初始化 cocos2d::Layer，并创建 UI 元素（如撤销按钮）。
     *
     * @return bool 初始化成功返回 true，否则返回 false。
     */
    virtual bool init();

private:
    /**
     * @brief 卡牌视图集合
     *
     * 存储当前场景中所有活动的 CardView 指针。
     */
    std::vector<CardView*> _cardViews;

    /**
     * @brief 卡牌点击回调
     */
    std::function<void(int)> _cardClickCallback;

    /**
     * @brief 撤销按钮点击回调
     */
    std::function<void()> _undoClickCallback;

    void _createBackgrounds();
    void _createUndoButton();
};

#endif // __GAME_VIEW_H__

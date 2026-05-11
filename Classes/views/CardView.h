/**
 * @file CardView.h
 * @brief Visual representation of a card.
 */

#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <memory>
#include <functional>

 /**
  * @class CardView
  * @brief 扑克牌视图类
  *
  * 功能描述：
  * 该类继承自 cocos2d::Sprite，用于在屏幕上显示扑克牌的图像。
  * 它负责根据 CardModel 的数据（花色、点数）渲染扑克牌，并处理用户的点击交互。
  *
  * 职责：
  * 1. 渲染扑克牌的背景、花色和数字。
  * 2. 捕获并处理触摸事件，通过回调通知外部。
  * 3. 提供移动等基础动画功能。
  *
  * 使用场景：
  * 适用于任何需要显示扑克牌并与之交互的卡牌游戏场景，如发牌、出牌等。
  */
class CardView : public cocos2d::Sprite {
public:
    /**
     * @brief 创建 CardView 实例
     *
     * 静态工厂方法，用于创建并初始化一个 CardView 对象。
     *
     * @param model 指向 CardModel 的共享指针，包含扑克牌的数据（花色、点数）。
     * @return CardView* 返回创建成功的 CardView 指针，如果创建失败则返回 nullptr。
     */
    static CardView* create(const std::shared_ptr<CardModel>& model);

    /**
     * @brief 设置点击回调函数
     *
     * 当扑克牌被点击时，会调用此回调函数。
     *
     * @param cb 回调函数，参数为扑克牌的 ID (int)。
     */
    void setClickCallback(const std::function<void(int)>& cb) { _onClickCallback = cb; }

    /**
     * @brief 获取扑克牌 ID
     *
     * 获取当前视图绑定的扑克牌模型的 ID。
     *
     * @return int 返回扑克牌的 ID，如果模型为空则返回 -1。
     */
    int getCardId() const { return _model ? _model->getId() : -1; }

    /**
     * @brief 播放移动动画
     *
     * 将扑克牌从当前位置移动到指定的目标位置。
     *
     * @param targetPos 目标位置 (cocos2d::Vec2)。
     */
    void playMoveTo(const cocos2d::Vec2& targetPos);

    /**
     * @brief 处理触摸结束事件
     *
     * 响应触摸结束的操作，通常用于触发点击回调。
     *
     * @param touch 触摸对象信息。
     * @param event 事件对象信息。
     */
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

protected:
    /**
     * @brief 初始化 CardView
     *
     * @param model 扑克牌数据模型。
     * @return bool 初始化成功返回 true，否则返回 false。
     */
    bool init(const std::shared_ptr<CardModel>& model);

private:
    /**
     * @brief 设置精灵纹理或组合
     *
     * 尝试加载整张扑克牌的纹理，如果失败则尝试组合背景、花色和数字。
     *
     * @return bool 设置成功返回 true。
     */
    bool _setupSprite();

    /**
     * @brief 组合扑克牌元素
     *
     * 当没有整张扑克牌纹理时，将数字和花色精灵添加到背景上。
     */
    void _setupComposition();

    /**
     * @brief 设置触摸监听器
     *
     * 注册触摸事件监听器，以响应用户的点击操作。
     */
    void _setupTouchListener();

    /**
     * @brief 扑克牌数据模型
     *
     * 存储扑克牌的逻辑数据，如花色、点数等。
     */
    std::shared_ptr<CardModel> _model;

    /**
     * @brief 点击回调函数
     *
     * 当检测到有效点击时调用的函数对象。
     */
    std::function<void(int)> _onClickCallback;
};

#endif // __CARD_VIEW_H__

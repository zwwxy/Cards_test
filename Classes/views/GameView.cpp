/**
 * @file GameView.cpp
 * @brief Implementation of GameView.
 */

#include "GameView.h"
#include "utils/ResolutionUtils.h"

 // 创建 GameView 实例
 // 使用 new (std::nothrow) 分配内存，防止异常
 // 调用 init 进行初始化，如果失败则释放内存
GameView* GameView::create() {
    GameView* view = new (std::nothrow) GameView();
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

// 初始化 GameView
// 创建并添加撤销按钮
bool GameView::init() {
    if (!Layer::init()) return false;

    _createBackgrounds();
    _createUndoButton();

    return true;
}

void GameView::_createBackgrounds() {
    // 添加背景区域
    // 堆牌区背景 (紫色)
    auto stackBg = cocos2d::LayerColor::create(
        cocos2d::Color4B(128, 0, 128, 255), // Purple
        ResolutionUtils::kStackWidth,
        ResolutionUtils::kStackHeight
    );
    stackBg->setPosition(cocos2d::Vec2(0, 0));
    this->addChild(stackBg, -1); // Z-order -1 to be behind cards

    // 主牌区背景 (棕色)
    auto playfieldBg = cocos2d::LayerColor::create(
        cocos2d::Color4B(139, 69, 19, 255), // SaddleBrown
        ResolutionUtils::kPlayfieldWidth,
        ResolutionUtils::kPlayfieldHeight
    );
    playfieldBg->setPosition(cocos2d::Vec2(0, ResolutionUtils::kStackHeight));
    this->addChild(playfieldBg, -1);
}

void GameView::_createUndoButton() {
    // 创建撤销按钮
    // 尝试使用图片创建 MenuItemImage
    cocos2d::MenuItem* undoItem = cocos2d::MenuItemImage::create(
        "res/undo_normal.png",
        "res/undo_selected.png",
        [this](cocos2d::Ref* sender) {
            if (_undoClickCallback) _undoClickCallback();
        }
    );

    // 如果图片加载失败（宽度为0），则回退使用文本标签
    if (undoItem->getContentSize().width == 0) {
        auto label = cocos2d::Label::createWithSystemFont(u8"回退", "fonts/Songti.ttf", 48);
        // 使用 MenuItemLabel 替代 MenuItemImage
        auto labelItem = cocos2d::MenuItemLabel::create(label, [this](cocos2d::Ref* sender) {
            if (_undoClickCallback) _undoClickCallback();
            });
        undoItem = labelItem;
    }

    // 设置按钮位置并添加到菜单中
    // 放置在堆牌区右侧垂直居中位置
    undoItem->setPosition(cocos2d::Vec2(ResolutionUtils::kStackWidth - 150, ResolutionUtils::kStackHeight / 2));
    auto menu = cocos2d::Menu::create(undoItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    addChild(menu);
}

// 初始化游戏 UI
// 根据 GameModel 的状态，清除旧视图并重新创建所有卡牌视图
void GameView::initUI(const std::shared_ptr<GameModel>& model) {
    if (!model) return;

    // 清除现有的卡牌视图
    for (auto cv : _cardViews) {
        cv->removeFromParent();
    }
    _cardViews.clear();

    // 添加游戏区域的卡牌
    for (const auto& card : model->getPlayfieldCards()) {
        addCardView(card);
    }

    // 添加堆叠区的所有卡牌
    for (const auto& card : model->getStackCards()) {
        addCardView(card);
    }
}

// 添加单个卡牌视图
// 创建 CardView，设置回调，添加到场景并保存到列表
void GameView::addCardView(const std::shared_ptr<CardModel>& card) {
    auto cv = CardView::create(card);
    cv->setClickCallback(_cardClickCallback);
    addChild(cv);
    _cardViews.push_back(cv);
}

// 根据 ID 获取卡牌视图
// 遍历 _cardViews 列表查找匹配的 ID
CardView* GameView::getCardViewById(int cardId) {
    for (auto cv : _cardViews) {
        if (cv->getCardId() == cardId) return cv;
    }
    return nullptr;
}

// 播放卡牌匹配动画
// 找到对应卡牌，提升其 Z 轴顺序，并播放移动动画
void GameView::playMatchAnimation(int cardId, const cocos2d::Vec2& toPos) {
    auto cv = getCardViewById(cardId);
    if (cv) {
        cv->setLocalZOrder(100); // 确保移动时在最上层
        cv->playMoveTo(toPos);
    }
}

// 播放撤销移动动画
// 将卡牌瞬间移动到起始位置，然后播放移动到目标位置的动画
void GameView::playReverseMove(int cardId, const cocos2d::Vec2& fromPos, const cocos2d::Vec2& toPos) {
    auto cv = getCardViewById(cardId);
    if (cv) {
        cv->setPosition(fromPos);
        cv->playMoveTo(toPos);
    }
}

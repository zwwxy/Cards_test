#include "CardView.h"
#include "configs/CardResConfig.h"

USING_NS_CC;

// 获取数字图片的路径
// 根据花色决定颜色（方块和红桃为红色，其他为黑色）
// 根据点数决定显示的字符（1->A, 11->J, 12->Q, 13->K）
static std::string getBigNumberFilename(int face, CardSuitType suit) {
    std::string color = "black";
    if (suit == CardSuitType::CST_DIAMONDS || suit == CardSuitType::CST_HEARTS) {
        color = "red";
    }

    std::string faceStr;
    switch (face) {
    case 1: faceStr = "A"; break;
    case 11: faceStr = "J"; break;
    case 12: faceStr = "Q"; break;
    case 13: faceStr = "K"; break;
    default: faceStr = std::to_string(face); break;
    }

    return "res/number/big_" + color + "_" + faceStr + ".png";
}

static std::string getSmallNumberFilename(int face, CardSuitType suit) {
    std::string color = "black";
    if (suit == CardSuitType::CST_DIAMONDS || suit == CardSuitType::CST_HEARTS) {
        color = "red";
    }

    std::string faceStr;
    switch (face) {
    case 1: faceStr = "A"; break;
    case 11: faceStr = "J"; break;
    case 12: faceStr = "Q"; break;
    case 13: faceStr = "K"; break;
    default: faceStr = std::to_string(face); break;
    }

    return "res/number/small_" + color + "_" + faceStr + ".png";
}

// 获取花色图片的路径
static std::string getSuitFilename(CardSuitType suit) {
    switch (suit) {
    case CardSuitType::CST_CLUBS: return "res/suits/club.png";
    case CardSuitType::CST_DIAMONDS: return "res/suits/diamond.png";
    case CardSuitType::CST_HEARTS: return "res/suits/heart.png";
    case CardSuitType::CST_SPADES: return "res/suits/spade.png";
    default: return "";
    }
}

// 创建 CardView 实例
// 使用 new (std::nothrow) 分配内存，防止异常
// 调用 init 进行初始化，如果失败则释放内存
CardView* CardView::create(const std::shared_ptr<CardModel>& model) {
    CardView* view = new (std::nothrow) CardView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

// 初始化 CardView
// 保存模型数据，设置精灵纹理，设置位置，并添加触摸监听
bool CardView::init(const std::shared_ptr<CardModel>& model) {
    if (!model) return false;
    _model = model;

    // 设置精灵显示内容
    if (!_setupSprite()) return false;

    // 设置锚点为正中间
    setAnchorPoint(Vec2(0.5f, 0.5f));

    // 设置初始位置
    setPosition(_model->getPosition());

    // 设置触摸事件监听
    _setupTouchListener();

    return true;
}


// 优先尝试加载整张卡牌的纹理
// 如果失败，则加载通用背景并组合数字和花色
bool CardView::_setupSprite() {
    std::string texture = CardResConfig::getFrontTexture(_model->getFace(), _model->getSuit());

    if (Sprite::initWithFile(texture)) {
        return true;
    }

    // 尝试使用通用背景进行组合
    if (!Sprite::initWithFile("res/card_general.png")) {
        if (!Sprite::init()) return false;
    }

    _setupComposition();
    return true;
}

// 组合卡牌元素（数字和花色）
// 当没有整张卡牌资源时，动态将数字和花色贴图添加到背景上
void CardView::_setupComposition() {
    auto size = getContentSize();

    int face = static_cast<int>(_model->getFace());
    CardSuitType suit = _model->getSuit();

    // 1. 左上角小数字
    std::string smallNumberImg = getSmallNumberFilename(face, suit);
    auto smallNumberSprite = Sprite::create(smallNumberImg);
    if (smallNumberSprite) {
        smallNumberSprite->setAnchorPoint(Vec2(0, 1)); // 左上角
        smallNumberSprite->setPosition(Vec2(10, size.height - 10));\
        addChild(smallNumberSprite, 2);
    }

    // 2. 右上角花色
    std::string suitImg = getSuitFilename(suit);
    auto suitSprite = Sprite::create(suitImg);
    if (suitSprite) {
        suitSprite->setAnchorPoint(Vec2(1, 1)); // 右上角
        suitSprite->setPosition(Vec2(size.width - 10, size.height - 10)); \
        addChild(suitSprite, 2);
    }

    // 3. 下方中间大数字
    std::string bigNumberImg = getBigNumberFilename(face, suit);
    auto bigNumberSprite = Sprite::create(bigNumberImg);
    if (bigNumberSprite) {
        bigNumberSprite->setAnchorPoint(Vec2(0.5f, 0)); // 下中
        bigNumberSprite->setPosition(Vec2(size.width / 2, 20));
        addChild(bigNumberSprite, 1);
    }
}


// 设置触摸监听器
// 创建单点触摸监听器，处理点击事件
void CardView::_setupTouchListener() {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // 吞噬触摸，防止穿透

    // 触摸开始：检查触摸点是否在卡牌范围内
    listener->onTouchBegan = [this](cocos2d::Touch* t, cocos2d::Event* e) {
        // 使用 convertToNodeSpace 将世界坐标转换为节点局部坐标
        // 这样可以正确处理节点的锚点、缩放和旋转
        Vec2 localPoint = this->convertToNodeSpace(t->getLocation());
        Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);

        if (rect.containsPoint(localPoint)) {
            return true;
        }
        return false;
    };

    // 触摸结束：再次检查范围，确认点击有效，触发回调
    listener->onTouchEnded = [this](cocos2d::Touch* t, cocos2d::Event* e) {
        Vec2 localPoint = this->convertToNodeSpace(t->getLocation());
        Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);

        if (rect.containsPoint(localPoint)) {
            if (_onClickCallback) {
                _onClickCallback(_model->getId());
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// 播放移动动画
// 使用 MoveTo 动作在 1.0 秒内移动到目标位置
void CardView::playMoveTo(const cocos2d::Vec2& targetPos) {
    runAction(cocos2d::MoveTo::create(1.0f, targetPos));
}

// 处理触摸结束事件（备用接口）
void CardView::onTouchEnded(Touch* touch, Event* event) {
    if (_onClickCallback) {
        _onClickCallback(_model->getId());
    }
}

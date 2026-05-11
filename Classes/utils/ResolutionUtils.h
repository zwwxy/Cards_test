/**
 * @file ResolutionUtils.h
 * @brief Utility for handling screen resolution and scaling.
 */

#ifndef __RESOLUTION_UTILS_H__
#define __RESOLUTION_UTILS_H__

#include "cocos2d.h"

 /**
  * @class ResolutionUtils
  * @brief Helper class to setup design resolution.
  */
class ResolutionUtils {
public:
    // 设计分辨率
    static constexpr int kDesignWidth = 1080;
    static constexpr int kDesignHeight = 2080;

    // 区域尺寸定义
    static constexpr int kPlayfieldWidth = 1080;
    static constexpr int kPlayfieldHeight = 1500;

    static constexpr int kStackWidth = 1080;
    static constexpr int kStackHeight = 580;

    /**
     * @brief Sets up the design resolution size and policy.
     */
    static void setupResolution() {
        auto director = cocos2d::Director::getInstance();
        auto glview = director->getOpenGLView();
        if (!glview) {
            glview = cocos2d::GLViewImpl::create("Card Game");
            director->setOpenGLView(glview);
        }

        glview->setDesignResolutionSize(kDesignWidth, kDesignHeight, ResolutionPolicy::FIXED_WIDTH);

        // 设置窗口大小（仅桌面平台有效）
        // 分辨率和窗口大小同步
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//        glview->setFrameSize(kDesignWidth, kDesignHeight);
//#endif
    }
};

#endif // __RESOLUTION_UTILS_H__
/**
 * @file LevelConfigLoader.h
 * @brief 关卡配置加载器类
 *
 * 负责从 JSON 文件中加载关卡配置数据。
 */

#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "LevelConfig.h"
#include "cocos2d.h"
#include "json/document.h"
#include <memory>
#include <string>

 /**
  * @class LevelConfigLoader
  * @brief 关卡配置加载器
  *
  * 提供静态方法来加载和解析关卡配置文件。
  */
class LevelConfigLoader {
public:
    /**
     * @brief 加载关卡配置
     *
     * 读取预定义的 JSON 配置文件（Playfield.json 和 Stack.json），
     * 解析并构建 LevelConfig 对象。如果文件不存在或解析失败，将创建测试用的假数据。
     *
     * @param levelId 关卡 ID（当前版本未使用，加载固定文件）
     * @return std::shared_ptr<LevelConfig> 加载完成的关卡配置对象指针
     */
    static std::shared_ptr<LevelConfig> loadLevelConfig(int levelId) {
        auto config = std::make_shared<LevelConfig>();

        _loadCardsFromFile("configs/Playfield.json", config->playfield);
        _loadCardsFromFile("configs/Stack.json", config->stack);

        if (config->playfield.empty() && config->stack.empty()) {
            cocos2d::log("Config empty, using dummy");
            return _createDummyConfig();
        }

        return config;
    }

private:
    /**
     * @brief 从 JSON 文件加载卡牌列表
     *
     * 辅助函数，读取指定路径的 JSON 文件，解析其中的卡牌数组，
     * 并填充到输出向量中。
     *
     * @param filename JSON 文件路径
     * @param outList 用于存储解析结果的 CardConfig 向量引用
     */
    static void _loadCardsFromFile(const std::string& filename, std::vector<CardConfig>& outList) {
        if (!cocos2d::FileUtils::getInstance()->isFileExist(filename)) {
            cocos2d::log("Config file not found: %s", filename.c_str());
            return;
        }

        std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
        rapidjson::Document doc;
        doc.Parse(content.c_str());

        if (doc.HasParseError() || !doc.IsArray()) {
            cocos2d::log("JSON parse error or not array in %s", filename.c_str());
            return;
        }

        for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
            const auto& item = doc[i];
            CardConfig card;
            if (item.HasMember("CardFace")) card.cardFace = item["CardFace"].GetInt()+1;
            if (item.HasMember("CardSuit")) card.cardSuit = static_cast<CardSuitType>(item["CardSuit"].GetInt());
            if (item.HasMember("Position") && item["Position"].IsObject()) {
                const auto& pos = item["Position"];
                if (pos.HasMember("x") && pos.HasMember("y")) {
                    card.position = cocos2d::Vec2(pos["x"].GetFloat(), pos["y"].GetFloat());
                }
            }
            outList.push_back(card);
        }
    }

    /**
     * @brief 创建测试用的假配置
     *
     * 当配置文件加载失败时调用，生成一组默认的卡牌数据用于测试。
     *
     * @return std::shared_ptr<LevelConfig> 包含测试数据的配置对象
     */
    static std::shared_ptr<LevelConfig> _createDummyConfig() {
        auto config = std::make_shared<LevelConfig>();
        // Add some dummy cards
        // Playfield
        for (int i = 0; i < 5; ++i) {
            CardConfig c;
            c.cardFace = static_cast<int>(CardFaceType::CFT_ACE) + i;
            c.cardSuit = CardSuitType::CST_HEARTS;
            c.position = cocos2d::Vec2(200 + i * 100, 800);
            config->playfield.push_back(c);
        }
        // Stack
        for (int i = 0; i < 10; ++i) {
            CardConfig c;
            c.cardFace = static_cast<int>(CardFaceType::CFT_TWO) + i;
            c.cardSuit = CardSuitType::CST_CLUBS;
            c.position = cocos2d::Vec2(500, 200); // Stack position
            config->stack.push_back(c);
        }
        return config;
    }
};

#endif // __LEVEL_CONFIG_LOADER_H__

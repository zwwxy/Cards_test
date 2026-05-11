/**
 * @file GameTypes.h
 * @brief Definitions of enums and types used in the game.
 */

#ifndef __GAME_TYPES_H__
#define __GAME_TYPES_H__

 /**
  * Card Suit Types
  */
enum class CardSuitType {
    CST_NONE = -1,
    CST_CLUBS = 0,      // 梅花
    CST_DIAMONDS,       // 方块
    CST_HEARTS,         // 红桃
    CST_SPADES,         // 黑桃
    CST_NUM_CARD_SUIT_TYPES    // 牌型数量
};

/**
 * Card Face Types
 */
enum class CardFaceType {
    CFT_NONE = -1,  // NONE 无效牌面
    CFT_ACE = 0,    // ACE A
    CFT_TWO,        // TWO 2
    CFT_THREE,      // THREE 3
    CFT_FOUR,       // FOUR 4
    CFT_FIVE,       // FIVE 5
    CFT_SIX,        // SIX 6
    CFT_SEVEN,      // SEVEN 7
    CFT_EIGHT,      // EIGHT 8
    CFT_NINE,       // NINE 9
    CFT_TEN,        // TEN 10
    CFT_JACK,       // JACK J
    CFT_QUEEN,      // QUEEN Q
    CFT_KING,       // KING K
    CFT_NUM_CARD_FACE_TYPES     // 牌面数量
};

/**
 * Card State in the game
 */
enum class CardState {
    IN_STACK,       // 在堆叠区
    IN_PLAYFIELD    // 在牌桌上
};

#endif // __GAME_TYPES_H__

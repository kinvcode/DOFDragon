#ifndef BASEADDRESS_H
#define BASEADDRESS_H

#define LEFT    0
#define RIGHT   1
#define UP      2
#define DOWN    3
#define RUN_MONSTER 0   // 打怪
#define RUN_ITEM    1   // 捡物品
#define RUN_DOOR    2   // 到传送门
#define RUN_BUILD   3   // 破坏建筑
#define RUN_TOWN    4   // 城镇移动
#define RUN_BUFF    5   // BUFF移动


#define C_ENCRYPT_BATE_KEY 0x1A // 加解密字节秘钥
#define C_ENCRYPT_WORD_KEY 0x1F2B // 加解密双字节秘钥
#define C_ENCRYPT_KEY 0x1F2A025C // 加解密秘钥
#define C_ENCRYPT_INT64_KEY 0x1F2A015C4BFA2B1C // 加解密八字节秘钥

#define C_USER_BASE 0x1AC790C // 人物指针基址
#define C_DECRYPT_BASE 0x1B08A10 // 解密基址
#define C_ENCRYPT_BASE 0x1B08A50 // 加密基址
#define C_ENCRYPT_PARAM1 0x1851600 //加密参数1
#define C_ENCRYPT_PARAM2 0x1851800 //加密参数2
#define C_USER_CUR_HP 0x2BEC // 人物当前血量偏移

#endif // BASEADDRESS_H

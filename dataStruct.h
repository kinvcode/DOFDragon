#pragma once
#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>
#include <QDateTime>
#include <QTimeZone>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <qthread.h>
#include <unordered_set>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

// opencv Mat对象。cols等于X,rows等于Y

// 二维坐标
typedef struct
{
    int x;
    int y;
}ROOMCOOR;


// 三维坐标
typedef struct
{
    int x;
    int y;
    int z;
}COORDINATE;

// 怪物坐标
typedef struct
{
    int x;
    int y;
    int z;
    __int64 p;
}MONSTERCOOR;

typedef struct
{
    int word;
    int area;
    int x;
    int y;
}TOWNCOOR;


// 图内对象数据结构
typedef struct
{
    __int64 p;
    int type;
    int camp;
    int code;
    int blood;
    int width;
    int height;
    int collision;
    int build;
    QString name;
    COORDINATE coor;
}ITEMOBJ;

// 怪物数据结构
typedef struct
{
    __int64 p;
    int type;
    int camp;
    int code;
    int blood;
}MONSTER;

// 功能开关结构
struct FUNCTIONSWITCH{
    bool equip_skill; // 启用装备技能功能
    bool equip_skill_on; // 装备技能开关
};

typedef struct{
    QString det;     // 检测模型
    QString rec;     // 识别模型
    QString charset; // 字典文件
    bool use_gpu;    // 是否使用GPU
    bool use_tensorrt;    // 是否使用tensorrt（暂时不可用）
    int limit_side_len;   // 图片最大长度
    double det_db_thresh; // 用于过滤DB预测的二值化图像，设置为0.-0.3对结果影响不明显(暂时不清楚是什么)
    double det_db_box_thresh;   // DB后处理过滤box的阈值，如果检测存在漏框情况，可酌情减小(类似可信度)
    double det_db_unclip_ratio; // 表示文本框的紧致程度，越小则文本框更靠近文本
    QString det_db_score_mode;  // slow:使用多边形框计算bbox score，fast:使用矩形框计算。矩形框计算速度更快，多边形框对弯曲文本区域计算更准确。(暂时用不到slow)
    bool use_dilation;  // 好像是一个图片扩大的算法，尽量开启
    int rec_batch_num;  // 一个图片中最多包含多少个文本框
    int rec_img_h;      // 识别图片高度
    int rec_img_w;      // 识别图片宽度
}OCRPARAM;

// 选择副本结构
struct SELDUNGEON {
    bool entered = false; // 是否已进入选择地图界面
};

// 游戏剧情任务结构
struct DNFJOB {
    int code = -1;		// 任务编号
    QString require;	// 任务要求:
    QString name;		// 任务名称
};

// 角色列表中的角色信息
struct ROLEINFO
{
    QString name;		// 角色名称
    QString server;		// 服务器
    int account;		// QQ账号
    int character;		// 角色职业
    int advancement;	// 角色转职
    int awakening;		// 觉醒次数
    int level;			// 角色等级
    int prestige;		// 角色名望
    int position;		// 角色位置
};

struct ROLEPANEL {
    bool entered = false; // 是否已进入选择角色界面
    int index = -1;		  // 当前人物索引
};

// 用于房间集合的结构
struct ROOMINFO {
    char direct = -1;   // 默认-1，无效的方向
    int entry_num = 0;  // 进入次数
    std::vector<int> room_codes; // 本房间的地图编号列表
    cv::Mat room_map;   // 当前房间地形
    cv::Mat build_map;  // 带有建筑物的地图
};

// BFS队列
struct queueNode
{
    ROOMCOOR coor;
};

// 房间顺图方向
struct ROOMDIRECT {
    ROOMCOOR coor;  // 房间坐标
    int direct;     // 该房间顺图方向
};

// 当前房间结构
struct CURRENTROOM {
    std::vector<ITEMOBJ> dungeon_object_list;	// 图内对象列表
    std::vector<ITEMOBJ> monster_list;          // 怪物列表
    std::vector<ITEMOBJ> item_list;             // 图内物品列表
    ROOMCOOR coordinate;                        // 房间位置
    ROOMCOOR buff;                              // BUFF位置
    bool room_has_urgent = false;               // 是否存在时空旋涡
    bool has_stone_tablet = false;              // 是否存在石碑
};

// 副本结构
struct DUNGEONINFO
{
    bool entered;				// 是否已进入图内
    bool clearance_complate;    // 是否通关
    bool has_secial_room;       // 存在特殊房间（时空旋涡）
    __int64 time_pointer;		// 时间指针
    __int64 door_pointer;		// 门型指针
    int map_code;				// 当前副本编号
    int last_direction;			// 上次过图方向
    int monster_max_blood;		// 怪物最大血量
    qint64 entered_time;        // 进入副本时间
    int start_equipments_num;   // 开始时装备数量
    int start_gold_num;         // 开始时金币数量
    int start_clearcube_num;    // 开始时无色数量
    int room_num;               // 房间数量
    int difficulty;             // 选图难度
    COORDINATE user_coordinate; // 人物坐标
    ROOMCOOR boos_room;         // BOSS房间位置
    CURRENTROOM current_room;	// 当前房间结构
    std::vector<std::vector<ROOMINFO>> rooms; // 所有房间的信息
    std::vector<__int64> balck_items; // 物品指针黑名单
};

// 城镇结构
struct TOWNINFO
{
    bool entered = false;   // 是否已进入城镇
    bool skill_learned = false; // 是否已经学完技能
    int world_id = 0;        // 大区域ID
    int area_id = 0;        // 小区域ID
    int fatigue = 0;        // 城镇疲劳
    cv::Mat map;            // 当前区域城镇地图
};

struct PATHNODE{
    std::vector<ROOMCOOR> paths;
    bool has_destroy;
};

// 当前任务数据结构
typedef struct
{
    int code1;
    int code2;
    int dungeon_id;
    int npc_id;
    int level;
}QUESTINFO;

// 当前角色数据结构
typedef struct
{
    int character; // 基础职业
    int character_class; // 转职职业
    int level;    // 等级
    int fatigue;  // 疲劳值
    int fame;     // 名望
    int backpack_weight; // 负重
    bool has_sub;        // 已开启辅助装备
    bool has_stones;     // 已开启魔法石
    bool has_earrings;   // 已开启耳环
}USERINFO;

// 图片模板匹配参数
typedef struct{
    QString img_name;
    QString tmp_src;
    QString tmp_name;
    int x_offset;
    int y_offset;
    int x_length;
    int y_length;
    ROOMCOOR tmp_coor;
    int tmp_width;
    int tmp_height;
}IMGMATCH;

typedef struct
{
    int code;
    int index;
}SKILLINDEX;

struct SKILLRANGE
{
    int distance;   // 技能最小距离
    int width;      // 技能宽度
    int height;     // 技能高度
};

struct SPACINGKEYBOARD
{
    int keyboard;   // 按键
    int time;       // 停顿时间
};

struct SKILLINFO {
    union {
        struct {
            int times;      // 可连续释放次数
            int time;       // 释放间隔时间（毫秒）
        } continuous;
        struct {
            int time;       // 蓄气时长（毫秒）
        } charge;
        struct {
            bool permanent; // 持久的
        } buff;
        struct {
            int monster;    // 召唤兽代码
            int limit;      // 召唤兽数量限制
        } summon;
        struct
        {
            int keyboard;   // 前置按键
            int time;       // 间隔时间
        } precombination;
        struct
        {
            int keyboard;   // 后置按键
            int time;       // 间隔时间
        } postcombination;
    } skill;
    int type;               // 技能类型：0普通攻击; 1普通技能； 2连续技能； 3蓄气技能； 4已删除； 5BUFF技能； 6召唤技能；7前置组合键；8后置组合键；
                            // 技能类型：
    int code;               // 技能代码
    SKILLRANGE range;       // 技能范围
};

typedef struct{
    SKILLINFO skill;
    int key;
}SKILLINFOKEY;

typedef struct
{
    int dungeon1; // 副本1
    int dungeon2; // 副本2
    int cur_dungeon; // 当前执行的副本
    int difficulty; // 副本难度
}CLEANDUNGEON;

typedef std::function<bool(cv::Rect &)> SeriaPortalFunction;

// 神界主线特殊图结构
struct DUSKYINFO
{
    bool first  = false;
    bool second = false;
    bool third  = false;
};

// 图形RECT
struct IMGRECT
{
    int x;
    int y;
    int width;
    int height;
};

// 起始结构
struct STARTANDEND
{
    __int64 start;
    __int64 end;
};

// 自定义BUFF结构
struct CUSTOMBUFF
{
    int overall_damage_value;
    int max_hp_value;
    int max_mp_value;
    int attack_speed_value;
    int casting_speed_value;
    int movement_speed_value;
};

// EPT Hook点
struct EPTHNODE{
    uint64_t hook_pfn; // HOOK页的物理帧（清理的时候需要卸载EPT HOOK）
    __int64 shadow_address; // 影子页虚拟地址（清理的时候需要释放掉）
};

#endif // DATASTRUCT_H

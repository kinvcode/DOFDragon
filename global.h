#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <windows.h>
#include <QPixmap>
//#include "paddle/paddleocr.h"
#include <QDebug>
//#include <QGuiApplication>
#include <QScreen>

class Global
{
public:
// 系统GDI
static double system_gdi;

// 游戏是否正在运行
static bool game_is_running;

// 游戏窗口句柄
static HWND game_hwnd;

// 游戏进程ID
static int game_pid;

// 游戏进程句柄
static HANDLE game_handle;

// 游戏进程CR3
static uint64_t game_cr3;

// 键鼠驱动句柄
static HANDLE MSDK_HANDLE;

// 是否虚拟机运行
static bool is_virtual;

// 游戏窗口是否是置顶状态
static bool game_is_top;

// 游戏窗口位置
static RECT game_rect;

// 游戏窗口真实size
static int game_width;
static int game_height;

// 自动运行
static bool autoplay;

// 自动线程指针
//static auto p_autoplay;

// 半自动模式
static bool semi_automatic;

// VT运行状态
static bool vt_running;

// 验证用户标记
static BYTE user_flag;

// 项目运行目录
static QString root_dir;
// OCR检测模型目录
static QString det_dir;
// OCR识别模型目录
static QString rec_dir;
// OCR字典目录
static QString keys_dir;
// 项目图片目录
static QString img_dir;
// 项目图片目录标准字符串
static std::string std_img_dir;

// HTTP token
static QString http_token;
// 验证用户名
static QString verify_user_name;
// HTTP token 刷新时间
static qint64 http_token_timestamp;
// 到期日期
static QString expired_date;
// 到期时间戳
static qint64 expired_timestamp;
// 脚本运行时间
static qint64 script_start_timestamp;
// 触发机制1的时间
static int trigger1_time;

// 获取当前系统GDI
static double getSystemGDI();
};

#endif // GLOBAL_H

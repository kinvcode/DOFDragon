#pragma once
#ifndef PROCESS_HELPER
#define PROCESS_HELPER
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <QMessageBox>
#include <processthreadsapi.h>
#include <QTime>
#include <QEventLoop>
#include <QCoreApplication>
#include <QTimer>
#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QFileInfo>
#include <QDir>

#include "helper/rand.h"
#include "vmp/VMProtectSDK.h"

#pragma comment  (lib, "Advapi32.lib")

class ProcessHelper
{
public:
    // 获取指定进程
    static int getProcessPID(QString Name);
    // 以指定进程的子进程方式运行
    static int asSystem(int Pid, char FileName[]);
    // 继承游戏进程
    static void asGameChild();
    // 事件处理
    static void handleEvents();
    // 程序延迟
    static void programDelay(int msec);
    // 程序随机延迟
    static void randomDelay(int min,int max);
    // 删除自身
    static bool deleteSelf();
    // 重命名自身
    static void renameSelf();
    // 命令行删除
    static bool deleteViaCommandLine(const QString &filePath);
    // 关闭指定PID的进程
    static bool killProcessByPID(qint64 pid);
};
#endif

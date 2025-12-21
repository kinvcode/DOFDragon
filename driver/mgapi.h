#ifndef MGAPI_H
#define MGAPI_H

#include <QString>
#include <QMutex>
#include <windows.h>
#include "inc.h"


class MGAPI
{
public:
    MGAPI();

    //
    //是否已加载驱动
    //
    static BOOL isLoadDriver();

    //
    //获取模块基址
    //
    static ULONG64 getModuleAddr(ULONG pid, const QString& moduleName);

    //
    //提交物理页,用于刚使用VirtualAlloc申请内存后，需要立刻写入
    //
    static BOOL probeRead(ULONG pid, ULONG64 addr);

    //
    //保护目标进程
    //
    static BOOL protect(ULONG pid);

    // 隐藏目标进程
    static BOOL hide(ULONG pid);

    // 恢复目标进程
    void restore();

    // 获取进程ID
    static int getPid(QString name);

    // 申请内存
    static INT64 alloc(ULONG pid,ULONG size);

    // 释放内存
    static void free(ULONG pid, ULONG64 addr);

    //-------------------------------READ--------------------------------------------------

    //
    //读内存 字节型(BYTE)
    //
    static BYTE readByte(ULONG pid, ULONG64 addr);

    //
    //读内存 短整数型(SHORT)
    //
    static SHORT readShort(ULONG pid, ULONG64 addr);

    //
    //读内存 整数型(INT)
    //
    static INT readInt(ULONG pid, ULONG64 addr);

    //
    //读内存 长整数型(INT64)
    //
    static INT64 readInt64(ULONG pid, ULONG64 addr);

    //
    //读内存 浮点型(FLOAT)
    //
    static FLOAT readFloat(ULONG pid, ULONG64 addr);

    //
    //读内存 字节集(Bytes)
    //
    static BOOL readBytes(ULONG pid, ULONG64 addr, _Out_ PBYTE pDataBuffer, ULONG size);

    //-------------------------------WRITE--------------------------------------------------

    //
    //写内存 字节型(BYTE)
    //
    static BOOL writeByte(ULONG pid, ULONG64 addr, _In_ BYTE pDataBuffer);

    //
    //写内存 短整数型(SHORT)
    //
    static BOOL writeShort(ULONG pid, ULONG64 addr, _In_ SHORT pDataBuffer);

    //
    //写内存 整数型(INT)
    //
    static BOOL writeInt(ULONG pid, ULONG64 addr, _In_ INT pDataBuffer);

    //
    //写内存 长整数型(INT64)
    //
    static BOOL writeInt64(ULONG pid, ULONG64 addr, _In_ INT64 pDataBuffer);

    //
    //写内存 浮点型(FLOAT)
    //
    static BOOL writeFloat(ULONG pid, ULONG64 addr, _In_ FLOAT pDataBuffer);

    //
    //写内存 字节集(Bytes)
    //
    static BOOL writeBytes(ULONG pid, ULONG64 addr, _In_ PBYTE pDataBuffer, ULONG size);

private:

    static BOOL readMemory(ULONG64 pid, ULONG64 addr, _Out_ PBYTE pDataBuffer, ULONG size);

    static BOOL writeMemory(ULONG64 pid, ULONG64 addr, _In_ PBYTE pDataBuffer, ULONG size);

    static BOOL allocMemory(ULONG pid, _Out_ PBYTE pDataBuffer, ULONG size);

    static BOOL freeMemory(ULONG pid, ULONG64 addr);
};

#endif // MGAPI_H

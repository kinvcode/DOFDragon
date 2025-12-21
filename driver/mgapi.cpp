#include "mgapi.h"

QMutex driver_read_mutex;

MGAPI::MGAPI() {}

BOOL MGAPI::isLoadDriver()
{
    DWORD data1 = 999;
    DWORD data2 = 0;

    if (readMemory((ULONG64)GetCurrentProcessId(),(ULONG64)&data1,(PBYTE)&data2,sizeof(DWORD)))
    {
        if (data1 == data2)
        {
            return TRUE;
        }
    }

    return FALSE;
}

ULONG64 MGAPI::getModuleAddr(ULONG pid, const QString& moduleName)
{
    BOOM_PROCESS_OPERA control = { 0 };

    WCHAR buf[MAX_PATH] = { 0 };

    control.tag = TAG_GETMODULE;
    control.pid = (ULONG64)pid;
    control.buf = (ULONG64)buf;

    if (!moduleName.isEmpty())
    {
        wcsncpy_s(buf, moduleName.toStdWString().c_str(), MAX_PATH);
    }

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return *(ULONG64*)&buf;
}

BOOL MGAPI::probeRead(ULONG pid, ULONG64 addr)
{
    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_PROBEREAD;
    control.pid = (ULONG64)pid;
    control.address = (ULONG64)addr;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return TRUE;
}

BOOL MGAPI::protect(ULONG pid)
{
    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_PROTECT;
    control.pid = (ULONG64)pid;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return TRUE;
}

BOOL MGAPI::hide(ULONG pid)
{
    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_HIDEPID;
    control.pid = (ULONG64)pid;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return TRUE;
}

void MGAPI::restore()
{
    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_RESTORE;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));
}

int MGAPI::getPid(QString name)
{
    int pid = 0;

    QByteArray ansiData = name.toLatin1();

    const char *ansiStr = ansiData.constData();

    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_GETPID;
    control.pid = (ULONG64)GetCurrentProcessId();
    control.buf = (ULONG64)ansiStr;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    int value = *(int*)ansiStr;

    if(value > 0)
    {
        pid = value;
    }

    return pid;
}

INT64 MGAPI::alloc(ULONG pid,ULONG size)
{
    INT64 buf = 0;
    allocMemory(pid, (PBYTE)&buf, size);
    return buf;
}

void MGAPI::free(ULONG pid, ULONG64 addr)
{
    freeMemory(pid, addr);
}

BYTE MGAPI::readByte(ULONG pid, ULONG64 addr)
{
    BYTE buf = '\0';
    readMemory(pid, addr, (PBYTE)&buf, sizeof(BYTE));
    return buf;
}

SHORT MGAPI::readShort(ULONG pid, ULONG64 addr)
{
    SHORT buf = 0;
    readMemory(pid, addr, (PBYTE)&buf, sizeof(SHORT));
    return buf;
}

INT MGAPI::readInt(ULONG pid, ULONG64 addr)
{
    INT buf = 0;
    readMemory(pid, addr, (PBYTE)&buf, sizeof(INT));
    return buf;
}

INT64 MGAPI::readInt64(ULONG pid, ULONG64 addr)
{
    INT64 buf = 0;
    readMemory(pid, addr, (PBYTE)&buf, sizeof(INT64));
    return buf;
}

FLOAT MGAPI::readFloat(ULONG pid, ULONG64 addr)
{
    FLOAT buf = 0.0f;
    readMemory(pid, addr, (PBYTE)&buf, sizeof(FLOAT));
    return buf;
}

BOOL MGAPI::readBytes(ULONG pid, ULONG64 addr, PBYTE pDataBuffer, ULONG size)
{
    return readMemory(pid, addr, pDataBuffer, size);
}

BOOL MGAPI::writeByte(ULONG pid, ULONG64 addr, BYTE pDataBuffer)
{
    return writeMemory(pid, addr, (PBYTE)&pDataBuffer, sizeof(BYTE));
}

BOOL MGAPI::writeShort(ULONG pid, ULONG64 addr, SHORT pDataBuffer)
{
    return writeMemory(pid, addr, (PBYTE)&pDataBuffer, sizeof(SHORT));
}

BOOL MGAPI::writeInt(ULONG pid, ULONG64 addr, INT pDataBuffer)
{
    return writeMemory(pid, addr, (PBYTE)&pDataBuffer, sizeof(INT));
}

BOOL MGAPI::writeInt64(ULONG pid, ULONG64 addr, INT64 pDataBuffer)
{
    return writeMemory(pid, addr, (PBYTE)&pDataBuffer, sizeof(INT64));
}

BOOL MGAPI::writeFloat(ULONG pid, ULONG64 addr, FLOAT pDataBuffer)
{
    return writeMemory(pid, addr, (PBYTE)&pDataBuffer, sizeof(FLOAT));
}

BOOL MGAPI::writeBytes(ULONG pid, ULONG64 addr, PBYTE pDataBuffer, ULONG size)
{
    return writeMemory(pid, addr, pDataBuffer, size);
}

BOOL MGAPI::readMemory(ULONG64 pid, ULONG64 addr, PBYTE pDataBuffer, ULONG size)
{
    // 锁定互斥量
    //QMutexLocker locker(&driver_read_mutex);

    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_READ;
    control.pid = (ULONG64)pid;
    control.address = (ULONG64)addr;
    control.buf = (ULONG64)pDataBuffer;
    control.size = size;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return TRUE;
}

BOOL MGAPI::writeMemory(ULONG64 pid, ULONG64 addr, PBYTE pDataBuffer, ULONG size)
{
    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_WRITE;
    control.pid = (ULONG64)pid;
    control.address = (ULONG64)addr;
    control.buf = (ULONG64)pDataBuffer;
    control.size = size;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return TRUE;
}

BOOL MGAPI::allocMemory(ULONG pid, PBYTE pDataBuffer, ULONG size)
{
    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_ALLOC;
    control.pid = (ULONG64)pid;
    control.buf = (ULONG64)pDataBuffer;
    control.size = size;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return TRUE;
}

BOOL MGAPI::freeMemory(ULONG pid, ULONG64 addr)
{
    BOOM_PROCESS_OPERA control = { 0 };

    control.tag = TAG_FREE;
    control.pid = (ULONG64)pid;
    control.address = (ULONG64)addr;

    RegSetValueEx(HKEY_LOCAL_MACHINE, L"", NULL, REG_BINARY, (byte*)&control, sizeof(control));

    return TRUE;
}


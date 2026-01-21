#pragma once
#ifndef MEM_H
#define MEM_H

#include <vector>
#include <string>
#include <QString>
#include "atlstr.h"
#include "global.h"
#include "baseAddress.h"
#include "process/process.h"
#include "driver/mgapi.h"

std::vector<byte> operator+(std::vector<byte> a, std::vector<byte> b);

namespace Mem
{
    bool readMemory(ULONG64 addr, PBYTE pDataBuffer, ULONG size);

    bool writeMemory(ULONG64 addr, PBYTE pDataBuffer, ULONG size);

    byte readByte(__int64 address);

    WORD readWord(__int64 address);

    bool writeByte(__int64 address,BYTE value);

    bool writeWord(__int64 address, WORD value);

    int readInt(__int64 address);

    bool writeInt(__int64 address, __int64 value);

    __int64 readLong(__int64 address);

    bool writeLong(__int64 address, __int64 value);

    float readFloat(__int64 address);

    bool writeFloat(__int64 address, float value);

    std::vector<byte> readByteArray(__int64 address, int length);

    bool writeByteArray(__int64 address, std::vector<byte> Data);

    QString readString(__int64 address, int length);

    bool writeString(__int64 address, QString text);

    std::vector<byte> wstringToBytes(std::wstring w_string);

    std::wstring bytesToWstring(std::vector<byte> data);

    float intToFloat(int value);

    std::vector<byte> makeByteArray(std::vector<byte> data);

    std::vector<byte> makeEmptyByteArray(int length);

    std::vector<byte> intToBytes(int length);

    std::vector<byte> longToBytes(__int64 length);

    std::vector<byte> reverseBytes(std::vector<byte> data);

    int memoryAssambly(std::vector<byte>asm_code);

    bool encrypt(uint32_t address, int value);

    bool encrypt(__int64 address, float f_value);

    int decrypt(int encrypt_address);

    __int64 decryptLong(__int64 address);

    __int64 allocMemory(ULONG size);

    void freeMemory(__int64 address);
};

#endif // MEM_H

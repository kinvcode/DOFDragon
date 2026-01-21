#include "mem.h"

//using namespace Global;

std::vector<byte> operator+(std::vector<byte> a, std::vector<byte> b)
{
    for (int i = 0; i < b.size(); i++)
    {
        a.push_back(b[i]);
    }

    b.clear();

    return a;
}

namespace Mem{

bool readMemory(ULONG64 addr, PBYTE pDataBuffer, ULONG size)
{
#ifdef VERIFY
    if(Global::user_flag == 1)
    {
        if (Global::game_handle == NULL || addr == 0)
        {
            return false;
        }

        if (ReadProcessMemory(Global::game_handle, (void*)addr, pDataBuffer, size, NULL))
        {
            return true;
        }
    }else{
        if(MGAPI::readBytes(Global::game_pid,addr,pDataBuffer,size))
        {
            return true;
        }
    }

#else
    if(MGAPI::readBytes(Global::game_pid,addr,pDataBuffer,size))
    {
        return true;
    }
#endif
    return false;
}

bool writeMemory(ULONG64 addr, PBYTE pDataBuffer, ULONG size)
{
#ifdef VERIFY
    if(Global::user_flag == 1)
    {
        if (Global::game_handle == NULL || addr == 0)
        {
            return false;
        }

        if (WriteProcessMemory(Global::game_handle, (void*)addr, pDataBuffer, size, NULL)) {
            return true;
        }
    }else{
        if(MGAPI::writeBytes(Global::game_pid,addr,pDataBuffer,size))
        {
            return true;
        }
    }
#else
    if(MGAPI::writeBytes(Global::game_pid,addr,pDataBuffer,size))
    {
        return true;
    }
#endif

    return false;
}

byte readByte(long long address)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return -1;
    }

    byte value;

    if (readMemory(address, &value, 1))
    {
        return value;
    }

    return -1;
}

bool writeWord(long long address, WORD value)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return false;
    }

    if (writeMemory(address, (byte*)&value, 2))
    {
        return true;
    }

    return false;
}

int readInt(long long address)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return -1;
    }

    int value;

    if (readMemory(address, (byte*)&value, 4))
    {
        return value;
    }

    return -1;
}

bool writeInt(long long address, long long value)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return false;
    }

    if (writeMemory(address, (byte*)&value, 4))
    {
        return true;
    }

    return false;
}

long long readLong(long long address)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return -1;
    }

    __int64 value;

    if (readMemory(address, (byte*)&value, 8))
    {
        return value;
    }

    return -1;
}

bool writeLong(long long address, long long value)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return false;
    }

    bool res = writeMemory(address, (byte*)&value, 8);

    return res;
}

float readFloat(long long address)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return -1;
    }

    float value;

    if (readMemory(address, (byte*)&value, 4))
    {
        return value;
    }

    return -1;
}

bool writeFloat(long long address, float value)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return false;
    }

    bool res = writeMemory(address, (byte*)&value, 4);

    return res;
}

std::vector<byte> readByteArray(long long address, int length)
{
    std::vector<byte> result;

    if (Global::game_pid == NULL|| address == 0)
    {
        return result;
    }

    byte* tempResult;
    tempResult = new byte[length];
    memset(tempResult, 0, length);

    readMemory(address,tempResult, length);

    result.resize(length);
    for (int i = 0; i < length; i++)
    {
        result[i] = tempResult[i];
    }

    delete[] tempResult;
    return result;
}

bool writeByteArray(long long address, std::vector<byte> Data)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return false;
    }

    int length;
    length = (int)Data.size();
    byte* val = new byte[length];
    for (int i = 0; i < length; i++)
    {
        val[i] = Data[i];
    }

    bool res = writeMemory(address,val, length);

    delete[] val;
    return res;
}

QString readString(long long address, int length)
{
    // if (Global::game_pid == NULL || address <= 0)
    // {
    //     return "";
    // }

    // // 分配内存时考虑 WCHAR 是宽字符（通常是 2 字节），所以分配的大小是 length/2
    // WCHAR* tempResult = new WCHAR[length / 2 + 1]; // +1 用于确保有足够的空间存储终止符
    // memset(tempResult, 0, (length / 2 + 1) * sizeof(WCHAR)); // 确保初始化为 0

    // readMemory(address,(byte*)tempResult, length);

    // // 查找终止符 '\0'
    // int actualLength = 0;
    // while (actualLength < length / 2 && tempResult[actualLength] != '\0')
    // {
    //     actualLength++;
    // }

    // // 构造 QString，只包含有效字符
    // QString text = QString::fromWCharArray(tempResult, actualLength);

    // delete[] tempResult;

    // return text;


    if (Global::game_pid == NULL || address <= 0)
    {
        return "";
    }

    // WCHAR* tempResult;
    // tempResult = new WCHAR[length / 2];
    // readMemory(address,(byte*)tempResult, length);
    // QString text = QString::fromWCharArray(tempResult);
    // delete[] tempResult;
    // return text;

    int count = 1;
    WORD w_value = 0;
    __int64 start_address = address;
    while (true)
    {
        w_value = readWord(address);

        if (w_value == 0)  // 遇到字符串结束符，终止读取
        {
            break;
        }

        if (count >= length)  // 限制最大长度
        {
            break;
        }

        address += 2;
        count ++;
    }

    WCHAR* tempResult;
    tempResult = new WCHAR[count];
    readMemory(start_address,(byte*)tempResult, count*2);
    QString text = QString::fromWCharArray(tempResult);
    delete[] tempResult;
    return text;
}

bool writeString(long long address, QString text)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return false;
    }

    const ushort* p_str = text.utf16();

    int length = text.length() * 2 + 2;
    bool res = writeMemory(address,(byte*)p_str, length);
    return res;
}

std::vector<byte> wstringToBytes(std::wstring w_string)
{
    std::vector<byte> data;
    // 获取宽字符串指针
    const wchar_t* ptr1 = w_string.c_str();
    // 转为字符指针
    byte* pChar = (byte*)ptr1;
    // 获取字符长度（不包含\0）;
    size_t leng = w_string.size() * 2;
    // 字符放入到容器
    for (size_t i = 0; i < leng; i++) {
        data.push_back((byte) * (pChar + i));
    }

    data.push_back(0);
    data.push_back(0);
    return data;
}

std::wstring bytesToWstring(std::vector<byte> data)
{
    // 声明宽字符串
    std::wstring wstr;
    // 获取字符长度（不包含\0）;
    size_t length = data.size() - 2;
    // 重新设置字符串大小
    wstr.resize(length / 2, 0);
    // 获取字符串指针
    byte* pByte = (byte*)wstr.data();
    // 填充字符串
    for (size_t i = 0; i < length; i++)
    {
        *pByte = data[i];
        pByte++;
    }
    return wstr;
}

std::vector<byte> makeByteArray(std::vector<byte> data)
{
    return data;
}

std::vector<byte> makeEmptyByteArray(int length)
{
    std::vector<byte> data;
    for (int i = 0; i < length; i++)
    {
        data.push_back(NULL);
    }
    return data;
}

std::vector<byte> intToBytes(int length)
{
    __int64  adr = (__int64)&length;
    std::vector<byte>c;
    for (size_t i = 0; i < 4; i++) {
        c.push_back(*(byte*)adr++);
    }
    return c;
}

std::vector<byte> longToBytes(long long length)
{
    __int64  adr = (__int64)&length;
    std::vector<byte>c;
    for (size_t i = 0; i < 8; i++) {
        c.push_back(*(byte*)adr++);
    }
    return c;
}

std::vector<byte> reverseBytes(std::vector<byte> data)
{
    std::vector<byte> new_data;
    int length = (int)data.size();
    for (int i = 0; i < length; i++)
    {
        new_data.push_back(data.back());
        data.pop_back();
    }
    return new_data;
}

int memoryAssambly(std::vector<byte> asm_code)
{
    // 汇编HOOK地址
    __int64 C_ASM_CALL = C_ASM_HOOK;
    // HOOK数据和原始数据
    std::vector<byte>HOOKdata, HOOKraw;

    //异步执行
    bool asyncExecute = false;

    //汇编中转,空白地址,判断地址
    __int64 asmTransfer, asm_code_address, logicAddress, asm_result, HOOKasm, HOOKret;

    asmTransfer      = C_EMPTY_ADDRESS;		   // HOOK代码
    logicAddress     = C_EMPTY_ADDRESS + 0x70; // 判断存放
    asm_result       = C_EMPTY_ADDRESS + 0x74; // 结果存放
    asm_code_address = C_EMPTY_ADDRESS + 0xC0; // 汇编地址

    // 此处使用锁
    if (asyncExecute) {
        return 0;
    }

    asyncExecute = true;
    HOOKasm = C_ASM_CALL;
    HOOKret = HOOKasm + 15; // HOOK返回位置

    HOOKraw = makeByteArray({0x55, 0x8B, 0xEC, 0xFF, 0x15, 0x88, 0xA5, 0x55, 0x01}); // 获取HOOK处原始代码

    HOOKdata = makeByteArray({0x55, 0x8B, 0xEC, 0xFF, 0x15, 0x88, 0xA5, 0x55, 0x01});
    HOOKdata = HOOKdata + makeByteArray({ 0x50, 0x55, 0xB8, 0x70, 0x30, 0x9E, 0x01, 0x81, 0x38, 0x01, 0x00, 0x00, 0x00});
    HOOKdata = HOOKdata + makeByteArray({0x75, 0x0E, 0xB8, 0xC0, 0x30, 0x9E, 0x01, 0xFF, 0xD0});
    HOOKdata = HOOKdata + makeByteArray({0xB9, 0x74, 0x30, 0x9E, 0x01, 0x89, 0x01 });
    HOOKdata = HOOKdata + makeByteArray({0xB8, 0x70, 0x30, 0x9E, 0x01, 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00});
    HOOKdata = HOOKdata + makeByteArray({0x5D, 0x58, 0xB9, 0x79, 0x51, 0x1A, 0x01});
    HOOKdata = HOOKdata + makeByteArray({0xFF, 0xE1, 0x90});

    // 如果HOOK代码没有数据则写入HOOK代码
    if (readInt(asmTransfer) == 0 || true)
    {
        writeByteArray(asmTransfer, HOOKdata);
    }

    // 汇编地址写入汇编数据
    writeByteArray(asm_code_address, asm_code + makeByteArray({ 195 })); // 写入执行CALL代码

    // 开始HOOK
    writeByteArray(HOOKasm, makeByteArray({0xB8, 0x00, 0x30, 0x9E, 0x01, 0xFF, 0xE0, 0x90, 0x90})); // HOOK汇编

    // 执行汇编
    writeInt(logicAddress, 1);
    while (readInt(logicAddress) == 1)
    {
        ProcessHelper::programDelay(5);
        ProcessHelper::handleEvents();
    }

    int call_result = readInt(asm_result);

    // 汇编代码重置
    writeByteArray(asm_code_address, makeEmptyByteArray((int)asm_code.size() + 16));

    return call_result;
}

bool encrypt(uint32_t address, int value)
{
    uint32_t ecx = address;
    int eax = readInt(C_ENCRYPT_BASE);
    eax++;
    int edx = eax;
    edx = edx >> 8;
    edx = (uint8_t)edx;
    edx = readInt(edx * 2 + C_ENCRYPT_PARAM1);
    writeInt(C_ENCRYPT_BASE,eax);
    edx = (int16_t)edx;
    eax = eax & 0xFF;
    edx = (int16_t)edx;
    edx = edx ^ (uint16_t)readInt(eax * 2 + C_ENCRYPT_PARAM2);
    int esi = value;
    eax = edx;
    eax = (uint16_t)eax;
    edx = esi >> 16;
    edx = (int16_t)edx;
    edx = edx + (int16_t)esi;
    edx ^= (int16_t)eax;
    int edi = edx;
    edx = eax;
    eax = eax << 16;
    eax |= edx;
    esi = value;
    eax ^= esi;
    esi = address + 0x8;
    writeInt(esi,eax);
    eax = readInt(address);
    esi = readInt(C_DECRYPT_BASE);
    ecx = edi;
    ecx = ecx << 16;
    ecx |= edx;
    edx = eax;
    edx = edx >> 16;
    edx = readInt(esi + edx * 4 + 0x24);
    eax = eax & 0xFFFF;
    writeInt(edx + eax * 4 + 0x2114,ecx);
    return true;
}

int decrypt(int encrypt_address) // 加密地址
{
    int encrypt_value = readInt(encrypt_address);
    int decrypt_param = readInt(C_DECRYPT_BASE);
    int edx = encrypt_value;

    edx = edx >> 0x10;
    edx = readInt(decrypt_param + edx * 4 + 0x24);
    int eax = encrypt_value & 0xFFFF;
    eax = readInt(edx + eax * 4 + 0x2114);
    edx = (uint16_t)eax;
    int decrypt_value = edx;
    decrypt_value = decrypt_value << 0x10;
    decrypt_value = decrypt_value | edx;
    decrypt_value = decrypt_value ^ readInt(encrypt_address + 0x8);

    return decrypt_value;
}

bool encrypt(long long address, float f_value)
{
#ifdef ISCHINA
    return writeInt(address, f_value);
#else
    float* f_p = &f_value;
    int value = (int)*(int*)f_p;

    value = value + 4;
    value = value ^ C_ENCRYPT_KEY;

    return writeInt(address, value);
#endif
}

float intToFloat(int value)
{
    int* p_x = &value;
    return (float)*(float*)p_x;
}

__int64 decryptLong(__int64 address)
{
#ifdef ISCHINA
    return readLong(address);
#else
    __int64 value = readLong(address);

    if (value == -1) {
        return -1;
    }

    value = value ^ C_ENCRYPT_INT64_KEY;
    value = value - 8;

    return value;
#endif
}

__int64 allocMemory(ULONG size)
{
    __int64 pointer = MGAPI::alloc(Global::game_pid, size);
    return pointer;
}

void freeMemory(__int64 address)
{
    MGAPI::free(Global::game_pid, address);
}

bool writeByte(long long address, BYTE value)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return false;
    }

    if (writeMemory(address, (byte*)&value, 1))
    {
        return true;
    }

    return false;
}

WORD readWord(long long address)
{
    if (Global::game_pid == NULL || address == 0)
    {
        return -1;
    }

    WORD value;

    if (readMemory(address, (byte*)&value, 2))
    {
        return value;
    }

    return -1;
}

}

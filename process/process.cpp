#include "process.h"

int ProcessHelper::getProcessPID(QString Name)
{
    VMProtectBegin("__FUNCTION__");
    LPCWSTR process_name = (const wchar_t*) Name.utf16();
    PROCESSENTRY32 Processinformation = { 0 };
    BOOL Processhandle;
    HANDLE Processsnapshot;

    Processsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Processsnapshot == INVALID_HANDLE_VALUE) {
        QMessageBox::warning(NULL,"幻境龙","创建进程快照失败");
        return 0;
    }

    Processinformation.dwSize = sizeof(PROCESSENTRY32);
    Processhandle = Process32First(Processsnapshot, &Processinformation);
    while (Processhandle)
    {
        if (lstrcmpi(process_name, Processinformation.szExeFile) == 0)
        {
            CloseHandle(Processsnapshot);

            return Processinformation.th32ProcessID;
        }
        Processhandle = Process32Next(Processsnapshot, &Processinformation);
    }
    CloseHandle(Processsnapshot);

    return 0;

    VMProtectEnd();
}

int ProcessHelper::asSystem(int Pid, char FileName[])
{
    VMProtectBegin("__FUNCTION__");
    BOOLEAN  nEn = 0;
    SIZE_T cbAttributeListSize = 0; // 接收所需缓存区大小（用于存放提权属性列表大小）
    HANDLE ProcessHandle = 0;
    PPROC_THREAD_ATTRIBUTE_LIST ps = 0;
    STARTUPINFOEXA si;
    ZeroMemory(&si, sizeof(si));
    si.StartupInfo.cb = sizeof(si);

    // 获取提权函数
    typedef LONG(*type_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
    type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege(20, 1, 0, &nEn);

    // 打开winlogon进程
    ProcessHandle = OpenProcess(2035711, FALSE, Pid);
    // 获取提权属性大小
    InitializeProcThreadAttributeList(NULL, 1, 0, &cbAttributeListSize);
    // 申请属性列表空间
    char* temp = new char[cbAttributeListSize];
    // 属性类型转换
    PPROC_THREAD_ATTRIBUTE_LIST AttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)temp;
    // ??
    InitializeProcThreadAttributeList(AttributeList, 1, 0, &cbAttributeListSize);
    // 更新权限为131072
    if (!UpdateProcThreadAttribute(AttributeList, 0, 131072, &ProcessHandle, sizeof(HANDLE), NULL, NULL))
    {
        QMessageBox::warning(NULL,"幻境龙","更新进程属性失败 !");
        ExitProcess(0);
    }
    //
    si.lpAttributeList = AttributeList;
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    si.StartupInfo.cb = 68;
    si.StartupInfo.dwFlags = 0;
    si.StartupInfo.wShowWindow = 1;

    // 创建目标进程
    if (CreateProcessAsUserA(NULL, 0, FileName, 0, 0, 0, 524288, 0, 0, (LPSTARTUPINFOA)&si, &pi))
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        QMessageBox::warning(NULL,"幻境龙","创建新进程失败!");
        ExitProcess(0);
    }
    CloseHandle(ProcessHandle);
    DeleteProcThreadAttributeList(AttributeList);
    delete[] temp;
    return pi.dwProcessId;

    VMProtectEnd();
}

void ProcessHelper::asGameChild()
{
    VMProtectBegin("__FUNCTION__");
    int dnf_pid = getProcessPID("DNF.exe");
    if (dnf_pid == 0)
    {
        QMessageBox::warning(NULL,"幻境龙","游戏尚未启动!");
        ExitProcess(0);
    }

    bool is_dnf_child = false;

    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);
    int self_pid = GetCurrentProcessId();

    if (Process32First(h, &pe)) {
        do {
            if (pe.th32ProcessID == self_pid) {
                if (dnf_pid == pe.th32ParentProcessID)
                {
                    is_dnf_child = true;
                }
            }
        } while (Process32Next(h, &pe));
    }

    if (!is_dnf_child) {
        // 关闭cmd
        TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, getProcessPID("cmd.exe")), 0);
        char path[MAX_PATH];
        memset(path, 0, MAX_PATH);
        DWORD nSize = ::GetModuleFileNameA(NULL, path, MAX_PATH);
        //WinExec("C:\\Windows\\System32\\cmd.exe", SW_HIDE);
        CreateProcess(L"C:\\Windows\\System32\\cmd.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, 0, 0);
        asSystem(dnf_pid, path);
        TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, getProcessPID("cmd.exe")), 0);
        ExitProcess(0);
    }

    VMProtectEnd();
}

void ProcessHelper::handleEvents()
{
    QApplication::processEvents();
}

void ProcessHelper::programDelay(int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void ProcessHelper::randomDelay(int min, int max)
{
    int rand = QRandomGenerator::global()->bounded(min, max + 1);
    programDelay(rand);
}

bool ProcessHelper::deleteSelf()
{
    VMProtectBegin("__FUNCTION__");
    // 删除源程序
    QString currentDir = QCoreApplication::applicationDirPath();
    QString filePath = currentDir + "/MicrosoftFastSearch.exe";
    QFile file(filePath);
    if (file.exists())
    {
        if (!file.remove()) {
            QMessageBox::information(NULL,"MicrosoftFastSearch","源程序删除失败!");
            ExitProcess(0);
        }
    }

    const wchar_t* NewStream = L":endlessparadox";
    WCHAR szPath[MAX_PATH * 2] = { 0 };

    // 获取当前可执行文件的路径
    if (GetModuleFileNameW(NULL, szPath, MAX_PATH * 2) == 0) {
        // std::wcerr << L"[!] GetModuleFileNameW fail , code is  " << GetLastError() << std::endl;
        QMessageBox::information(NULL,"MicrosoftFastSearch","GetModuleFileNameW fail!");
        return FALSE;
    }

    // 打开文件
    HANDLE hFile = CreateFileW(szPath,
                               DELETE | SYNCHRONIZE,
                               FILE_SHARE_READ,
                               NULL,
                               OPEN_EXISTING,
                               NULL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        // std::wcerr << L"[!] CreateFileW fail , code is " << GetLastError() << std::endl;
        QMessageBox::information(NULL,"MicrosoftFastSearch","CreateFileW fail!");
        return FALSE;
    }

    // 准备重命名信息
    SIZE_T sRename = sizeof(FILE_RENAME_INFO) + sizeof(wchar_t) * wcslen(NewStream);
    PFILE_RENAME_INFO pRename = (PFILE_RENAME_INFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sRename);
    if (!pRename) {
        CloseHandle(hFile);
        // std::wcerr << L"[!] HeapAlloc fail , code is " << GetLastError() << std::endl;
        QMessageBox::information(NULL,"MicrosoftFastSearch","HeapAlloc fail!");
        return FALSE;
    }

    pRename->FileNameLength = wcslen(NewStream) * sizeof(wchar_t);
    RtlCopyMemory(pRename->FileName, NewStream, pRename->FileNameLength);
    // std::wcout << L"[i] Renaming :$DATA to file data as " << NewStream << std::endl;

    if (!SetFileInformationByHandle(hFile, FileRenameInfo, pRename, sRename)) {
        // std::wcerr << L"[!] SetFileInformationByHandle fail, code is" << GetLastError() << std::endl;
        QMessageBox::information(NULL,"MicrosoftFastSearch","SetFileInformationByHandle fail!");
        CloseHandle(hFile);
        HeapFree(GetProcessHeap(), 0, pRename);
        return FALSE;
    }

    // std::wcout << L"[+] Completed" << std::endl;
    CloseHandle(hFile);

    // 打开文件以删除
    hFile = CreateFileW(szPath,
                        DELETE | SYNCHRONIZE,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING,
                        NULL, NULL);

    if (hFile == INVALID_HANDLE_VALUE && GetLastError() == 0) {
        // std::wcout << "free memory" << std::endl;
        HeapFree(GetProcessHeap(), 0, pRename);
        return TRUE;
    }

    FILE_DISPOSITION_INFO Delete = { 0 };
    Delete.DeleteFile = TRUE;
    // std::wcout << L"[+] Deleting ....." << std::endl;

    if (!SetFileInformationByHandle(hFile, FileDispositionInfo, &Delete, sizeof(Delete))) {
        // std::wcerr << L"[!] SetFileInformationByHandle fail, code is  " << GetLastError() << std::endl;
        QMessageBox::information(NULL,"MicrosoftFastSearch","SetFileInformationByHandle fail!");
        CloseHandle(hFile);
        HeapFree(GetProcessHeap(), 0, pRename);
        return FALSE;
    }

    CloseHandle(hFile);
    HeapFree(GetProcessHeap(), 0, pRename);
    // wprintf(L"[+] Done\n");
    qDebug() << "运行成功！";
    return TRUE;

    VMProtectEnd();
}

void ProcessHelper::renameSelf()
{
    VMProtectBegin("__FUNCTION__");
    // 获取程序名称
    QString name = QCoreApplication::applicationName();

    // 判断本程序名称
    if(name == "MicrosoftFastSearch")
    {
        // 获取当前程序运行目录
        QString currentDir = QCoreApplication::applicationDirPath();

        // 拼接文件路径
        QString filePath = currentDir + "/MicrosoftFastSearch.exe";

        // 使用 QFile 判断文件是否存在
        QFile file(filePath);
        if (!file.exists()) {
            QMessageBox::information(NULL,"MicrosoftFastSearch","主程序丢失!");
            qApp->quit();
        }

        // 新的程序名
        QString new_name = Rand::nameWithoutSymbol();

        // 获取自身程序的路径
        QString appPath = QCoreApplication::applicationFilePath();
        QFileInfo fileInfo(appPath);

        // 新的程序路径
        QString targetPath = currentDir + QDir::separator() + new_name;

        // 检查新文件是否已存在
        if (QFile::exists(targetPath)) {
            QMessageBox::information(NULL,"MicrosoftFastSearch","新文件名被暂用");
            ExitProcess(0);
        }

        // 打开源文件
        QFile sourceFile(appPath);
        if (!sourceFile.open(QIODevice::ReadOnly)) {
            QMessageBox::information(NULL,"MicrosoftFastSearch","无法打开源程序");
            ExitProcess(0);
        }

        // 打开目标文件
        QFile targetFile(targetPath);
        if (!targetFile.open(QIODevice::WriteOnly)) {
            QMessageBox::information(NULL,"MicrosoftFastSearch","无法打开目标程序");
            ExitProcess(0);
        }

        // 复制文件内容
        QByteArray data = sourceFile.readAll();
        targetFile.write(data);

        // 关闭文件
        sourceFile.close();
        targetFile.close();

        // 运行新程序
        QProcess process;
        QProcess::startDetached(targetPath);

        qDebug() << "启动重命名程序，原始程序即将关闭";
        ExitProcess(0);
    }
    VMProtectEnd();
}

bool ProcessHelper::deleteViaCommandLine(const QString &filePath)
{
    // 使用del命令（需要管理员）
    QString command = QString("cmd.exe");
    QStringList arguments;
    arguments << "/c" << "del" << "/f" << "/q" << "\"" + filePath + "\"";

    QProcess process;
    process.start(command, arguments);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        return true;
    }

    // 获取命令输出
    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    QMessageBox::critical(nullptr, "Vega",
                          QString("命令行删除失败：\n退出码：%1\n输出：%2\n错误：%3")
                              .arg(process.exitCode())
                              .arg(output)
                              .arg(error));

    return false;
}

bool ProcessHelper::killProcessByPID(qint64 pid)
{
    QStringList args;
    args << "/F"  // 强制终止
         << "/PID" << QString::number(pid);

    QProcess process;
    process.start("taskkill", args);
    process.waitForFinished();

    if(process.exitCode() !=0)
    {
        // PROCESS_TERMINATE 权限足够用于 TerminateProcess
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | SYNCHRONIZE,
                                      FALSE, pid);
        if (!hProcess)
            return false;

        DWORD exitCode = 1;       // 默认退出码
        // 强制终止
        BOOL ok = TerminateProcess(hProcess, exitCode);
        if (!ok)
        {
            CloseHandle(hProcess);
            return false;
        }

        // 等待进程真正退出，最多 3 秒
        DWORD waitCode = WaitForSingleObject(hProcess, 3000);

        CloseHandle(hProcess);

        // WAIT_OBJECT_0：进程确实终止了
        return (waitCode == WAIT_OBJECT_0);
    }

    return true;
}


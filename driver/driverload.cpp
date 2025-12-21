#include "driverload.h"
#include <QDebug>

DriverLoad::DriverLoad() {}

BOOL DriverLoad::enableDrvLoadPrivilege()
{
    VMProtectBegin("__FUNCTION__");

    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        return  FALSE;
    }
    if (!LookupPrivilegeValue(NULL, SE_LOAD_DRIVER_NAME, &sedebugnameValue))
    {
        CloseHandle(hToken);
        return FALSE;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
    {
        CloseHandle(hToken);
        return FALSE;
    }
    return TRUE;

    VMProtectEnd();
}

BOOL DriverLoad::installDriver(const QString& ServiceName, const QString& DriverPath)
{
    VMProtectBegin("__FUNCTION__");

    LPCWSTR lpServiceName = reinterpret_cast<LPCWSTR>(ServiceName.utf16());
    LPCWSTR lpDriverPath = reinterpret_cast<LPCWSTR>(DriverPath.utf16());

    SC_HANDLE sc_manage = NULL;
    SC_HANDLE sc_service = NULL;
    BOOL bRet = FALSE;

    do
    {
        sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
        if (NULL == sc_manage)
        {
            m_errorCode = GetLastError();
            break;
        }

        sc_service = CreateService(
            sc_manage,
            lpServiceName,
            lpServiceName,
            SERVICE_ALL_ACCESS,
            SERVICE_KERNEL_DRIVER,
            SERVICE_DEMAND_START, // 手动启动
            // SERVICE_AUTO_START, // 开机自启
            SERVICE_ERROR_IGNORE,
            lpDriverPath,
            NULL, NULL, NULL, NULL, NULL);

        if (NULL == sc_service)
        {
            m_errorCode = GetLastError();
            break;
        }

        bRet = TRUE;
    } while (0);

    if (sc_service != NULL)
    {
        CloseServiceHandle(sc_service);
        sc_service = NULL;
    }

    if (sc_manage != NULL)
    {
        CloseServiceHandle(sc_manage);
        sc_manage = NULL;
    }

    return bRet;

    VMProtectEnd();
}

BOOL DriverLoad::startDriver(const QString& ServiceName)
{
    VMProtectBegin("__FUNCTION__");

    LPCWSTR lpServiceName = reinterpret_cast<LPCWSTR>(ServiceName.utf16());

    SC_HANDLE sc_manage = NULL;
    SC_HANDLE sc_service = NULL;
    BOOL bRet = FALSE;

    do
    {
        sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (NULL == sc_manage)
        {
            m_errorCode = GetLastError();
            break;
        }

        sc_service = OpenService(sc_manage, lpServiceName, SERVICE_ALL_ACCESS);
        if (NULL == sc_service)
        {
            m_errorCode = GetLastError();
            break;
        }

        if (StartService(sc_service, NULL, NULL) == FALSE)
        {
            m_errorCode = GetLastError();
            break;
        }

        bRet = TRUE;
    } while (0);

    if (sc_service != NULL)
    {
        CloseServiceHandle(sc_service);
        sc_service = NULL;
    }

    if (sc_manage != NULL)
    {
        CloseServiceHandle(sc_manage);
        sc_manage = NULL;
    }

    return bRet;

    VMProtectEnd();
}

BOOL DriverLoad::stopDriver(const QString& ServiceName)
{
    VMProtectBegin("__FUNCTION__");

    LPCWSTR lpServiceName = reinterpret_cast<LPCWSTR>(ServiceName.utf16());

    SC_HANDLE sc_manage = NULL;
    SC_HANDLE sc_service = NULL;
    SERVICE_STATUS ss = { 0 };
    BOOL bRet = FALSE;

    do
    {
        sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (NULL == sc_manage)
        {
            m_errorCode = GetLastError();
            break;
        }

        sc_service = OpenService(sc_manage, lpServiceName, SERVICE_ALL_ACCESS);
        if (NULL == sc_service)
        {
            m_errorCode = GetLastError();
            break;
        }

        if (ControlService(sc_service, SERVICE_CONTROL_STOP, &ss) == FALSE)
        {
            m_errorCode = GetLastError();
            break;
        }

        bRet = TRUE;
    } while (0);

    if (sc_service != NULL)
    {
        CloseServiceHandle(sc_service);
        sc_service = NULL;
    }

    if (sc_manage != NULL)
    {
        CloseServiceHandle(sc_manage);
        sc_manage = NULL;
    }

    return bRet;

    VMProtectEnd();
}

BOOL DriverLoad::unInstallDriver(const QString& ServiceName)
{
    VMProtectBegin("__FUNCTION__");

    LPCWSTR lpServiceName = reinterpret_cast<LPCWSTR>(ServiceName.utf16());

    SC_HANDLE sc_manage = NULL;
    SC_HANDLE sc_service = NULL;
    BOOL bRet = FALSE;

    do
    {
        sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (sc_manage == NULL)
        {
            m_errorCode = GetLastError();
            break;
        }

        sc_service = OpenService(sc_manage, lpServiceName, SERVICE_ALL_ACCESS);
        if (sc_service == NULL)
        {
            m_errorCode = GetLastError();
            break;
        }

        if (DeleteService(sc_service) == FALSE)
        {
            m_errorCode = GetLastError();
            break;
        }

        bRet = TRUE;
    } while (0);

    if (sc_service != NULL)
    {
        CloseServiceHandle(sc_service);
        sc_service = NULL;
    }

    if (sc_manage != NULL)
    {
        CloseServiceHandle(sc_manage);
        sc_manage = NULL;
    }

    return bRet;

    VMProtectEnd();
}


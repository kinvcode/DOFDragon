#include "global.h"


double  Global::system_gdi = 1;
int     Global::game_pid   = 0;
HANDLE  Global::game_handle = NULL;
uint64_t Global::game_cr3   = 0;
HANDLE  Global::MSDK_HANDLE = NULL;
//AutoPlay* Global::p_autoplay = nullptr;
bool    Global::game_is_running = false;
bool    Global::is_virtual      = false;
bool    Global::game_is_top     = false;
bool    Global::autoplay        = false;
bool    Global::semi_automatic  = false;
bool    Global::vt_running      = false;
BYTE    Global::user_flag = 0;
int     Global::trigger1_time = 0;
RECT    Global::game_rect;
int     Global::game_width;
int     Global::game_height;
QString Global::root_dir;
QString Global::det_dir;
QString Global::rec_dir;
QString Global::keys_dir;
QString Global::img_dir;
std::string Global::std_img_dir;
QString Global::http_token;
QString Global::verify_user_name;
qint64 Global::http_token_timestamp;
QString Global::expired_date;
qint64 Global::expired_timestamp;
qint64 Global::script_start_timestamp;
double Global::getSystemGDI()
{

    // 获取窗口当前显示的监视器
    HWND hWnd = GetDesktopWindow();//根据需要可以替换成自己程序的句柄
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // 获取监视器逻辑宽度与高度
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);
    GetMonitorInfo(hMonitor, &miex);
    int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
    int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

    // 获取监视器物理宽度与高度
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;
    int cyPhysical = dm.dmPelsHeight;

    //缩放比例计算
    double horzScale = ((double)cxPhysical / (double)cxLogical);
    double vertScale = ((double)cyPhysical / (double)cyLogical);
    if(horzScale == vertScale){
        return horzScale;
    }else{
        return 0;
    }
}



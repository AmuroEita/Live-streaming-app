// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define _SINGLE_THREAD_CHECK

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

// Windows 头文件: 
#include <windows.h>
#include <shellapi.h>
#include <mmsystem.h>

//#define NOMINMAX
#define _WTYPES_NS 

#pragma warning( push ) 
#pragma warning( disable: 4302 ) //eg. warning C4302: “类型转换”: 从“LPCTSTR”到“WORD”截断
#pragma warning( disable: 4838 ) //eg. warning C4838: 从“int”转换到“UINT”需要收缩转换
#pragma warning( disable: 4800 ) //eg. warning C4800: “BOOL”: 将值强制为布尔值“true”或“false”(性能警告)
#pragma warning( disable: 4091 ) //eg. warning C4091: “typedef ”: 没有声明变量时忽略“tagGPFIDL_FLAGS”的左侧
#pragma warning( disable: 4996 ) //eg. warning C4996: 'strcpy': This function or variable may be unsafe

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

// STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <algorithm>
using namespace std;

// ATL && WTL
#include <atlbase.h>
#include <atlapp.h>
#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlcrack.h>      
#include <atlxml.h>
#include <atltime.h>

using namespace ATL;
extern CAppModule _Module;

#include <gdiplus.h>                //将GDI+头文件加入到工程中 
#pragma comment(lib, "gdiplus.lib") //将GDI+的lib文件加入到工程中 
using namespace Gdiplus;

#include <openssl\sha.h>

// extern "C"
// {
// #pragma warning(disable:4996)
// #include <openssl\sha.h>
// }

//#pragma comment(lib, "libssl32MDd.lib")  


// 3rdparty 
#include <uilib.h>
using namespace DuiLib;

//// include
#include "Utils.h"
#include "Singleton.h"
#include "commondef.h"
#include "ACCef/ACCef.h"

//pro include
#include "Common.h"

// bulid
#include <version.h>

// project
#include "Resource.h"
#include "SACModule.h"

#include "Base64.h"
#include "MD5.h"

//#include "Common.h"
#include "CosDef.h"
#include "CosAuthSign.h"

#include "fileHelper.h"
#include "IniHelper.h"
#include "PathHelper.h"

#include "RectHelper.h"

//baseclass
#include "Log.h"
#include "LogBase.h"
#include "CommandParse.h"
#include "TaskThreadBase.h"
#include "JsonParse.h"
#include "urlencode.h"

//project

#include "TimeMgr.h"
#include "LoginWindow.h"
#include "MessageUserdefine.h"
#include "LoginUserDataMgr.h"
#include "Mainframe.h"
#include "CefWndWrapManager.h"
#include "HelpWebWindow.h"
#include "UIListItemCust.h"

//有些窗口自带关闭按钮，自身窗口关闭的同时会触发OnFinalMessage，有的在消息中delete this，释放了内存，有的没有
//对于没有delete this的，要在这里delete
#define SAFE_RELEASE_WND(pWnd, deleteThis) if(pWnd)\
{\
    if (::IsWindow(*pWnd))\
        ::DestroyWindow(*pWnd);\
    if(deleteThis) delete pWnd;\
    pWnd=NULL;\
}

extern CSACModule g_SACMoudleApp;

string readFileIntoString(const char * filename);


#define App_Name L"微剪助手"

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// TODO:  在此处引用程序需要的其他头文件

// Rocket.cpp : 定义应用程序的入口点。
//
//
//#include "stdafx.h"
//#include "Rocket.h"
//#include "loginWindow.h"
//
//#define MAX_LOADSTRING 100
//
//// 全局变量:
//HINSTANCE hInst;                                // 当前实例
//WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
//WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: 在此处放置代码。
//
//    //add code
//	CPaintManagerUI::SetInstance(hInstance);
//
//	CPaintManagerUI::SetDefLang(L"zh_cn");
//
//	CLoginWindow LoginWindow;
//	{
//		DWORD dwStyle = WS_VISIBLE | WS_THICKFRAME;
//		LoginWindow.Create(NULL, L"", dwStyle, 0L);
//
//		LoginWindow.ShowWindow(true);
//		LoginWindow.CenterWindow();
//	}
//
//
//
//	int nRet = 0;
//	MSG msg = { 0 };
//	while ((nRet = ::GetMessage(&msg, NULL, 0, 0)) >= 0) {
//
//		if (!CPaintManagerUI::TranslateMessage(&msg)) {
//			::TranslateMessage(&msg);
//			::DispatchMessage(&msg);
//		}
//	}
//	return 0;
//
//}
//


#include "stdafx.h"
#include "Rocket.h"

#include "SACModule.h"
#include <TlHelp32.h>

CAppModule _Module;
CSACModule g_SACMoudleApp;

#define SAC_MUTEX_STRING	  L"{F5503400-290A-40C7-A9EF-F08F8FC37D8A}"

eProcessType GetProcessType(LPCWSTR lpwsCmdLine)
{
	eProcessType eType = eBrowserProcess;
	CCommandParse cmd;
	cmd.Parse(GetCommandLine());
	CCommand* pCmd = cmd.FindCommand(L"-type");
	if (NULL == pCmd)
	{
		eType = eBrowserProcess;
	}
	else
	{
		LPCWSTR pwsType = pCmd->GetArg(0);
		if (StrCmpI(pwsType, L"renderer") == 0)
		{
			eType = eRendererProcess;
		}
		else
		{
			eType = eOtherProcess;
		}
	}
	return eType;
}

#define MAIN_MUTEX_NAMEL L"D0632876-F7F3-4410-804A-D53C8BFDDA5F"
HANDLE hSACMutex = NULL;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	eProcessType eType = GetProcessType(GetCommandLine());
	if (eType == eBrowserProcess)
	{
		//if (lstrlen(lpCmdLine) > 0)
		{
			hSACMutex = ::CreateMutexW(NULL, FALSE, MUTEX_SAC_STRING);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
#ifndef DEBUG
				return 0;
#endif // DEBUG

			}
		}

		HRESULT hRes = OleInitialize(NULL);
		ATLASSERT(SUCCEEDED(hRes));
		::DefWindowProc(NULL, 0, 0, 0L);

		AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
		srand(GetTickCount());
		hRes = _Module.Init(NULL, hInstance);
		ATLASSERT(SUCCEEDED(hRes));
		AtlAxWinInit();
	}

	HRESULT hRes = OleInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
	srand(GetTickCount());
	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));
	AtlAxWinInit();

	g_SACMoudleApp.Init(eType);

	CComPtr<ICefManager> spCefMgr;
	HMODULE hMoudle = g_SACMoudleApp.GetCefModuleHandle();
	DllCreateObject(hMoudle, CLSID_CefManager, &spCefMgr);
	if (!spCefMgr)
	{
		::MessageBox(NULL, L"LoadCEF3MoudleFailed!!!", NULL, MB_OK);
	}
	else
	{
		spCefMgr->Init(hInstance, NULL);

		if (eBrowserProcess == eType)
		{
			g_SACMoudleApp.SetCefInstance(spCefMgr);
			CCefWndWrapManager::Init();
			int nRet = g_SACMoudleApp.DUIMain(hInstance, lpstrCmdLine, nCmdShow);
			CCefWndWrapManager::UnInit();
			CloseHandle(hSACMutex);
			hSACMutex = NULL;
		}
		spCefMgr->Shutdown();
	}

	//声网SDK也有卡死过，加上强杀保险一点，
	TerminateProcess(GetCurrentProcess(), 0);

	if (eBrowserProcess == eType)
	{
		g_SACMoudleApp.Uninit();
		CloseHandle(hSACMutex);
		hSACMutex = NULL;
		_Module.Term();
		OleUninitialize();
	}

	//TerminateProcess(GetCurrentProcess(), 0);

	return 0;
}
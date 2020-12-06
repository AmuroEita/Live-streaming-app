#include "stdafx.h"
#include "SACModule.h"
#include "Core/DuiLangManager.h"

#include <TlHelp32.h>

#define MKDIREXIST(x)		HELPER::MakeDirExist(x)
#include "MiniDump.h"
#include "IniHelper.h"
#include <atlenc.h>

CSACModule::CSACModule(void)
	:m_eProcessType(eBrowserProcess)
{
	
	
}

CSACModule::~CSACModule(void)
{
	
}

LPCWSTR LogFileName(CString& strName)
{
		LPCWSTR pwsPrefix = L"";

	SYSTEMTIME st = {0};
	GetLocalTime(&st);

	CString strFileName;
	strFileName.Format(MAIN_PROJECT_NAME L"_clsk12%s_%d_%02d%02d%02d_%02d%02d%02d_%03d.log",
		pwsPrefix, GetCurrentProcessId(),
		st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, 
		st.wMilliseconds); 

	WCHAR wszAppData[MAX_PATH] = {0};
	SHGetSpecialFolderPath(NULL, wszAppData, CSIDL_APPDATA, FALSE);

    CString strSubPath;
    strSubPath.Format(L"%s\\%s\\", PROGRAM_APPDATA_FLODER_NAME, SAVE_LOGS_FOLDER_NAME);

	PathAppend(wszAppData, strSubPath);
	strName = wszAppData + strFileName;
	return strName;
}


LPCWSTR CrashFileName(eProcessType eType, CString& strName)
{
	LPCWSTR pwsPrefix = L"";
	switch(eType)
	{
	case eBrowserProcess:	pwsPrefix = L"";		break;
	case eRendererProcess:	pwsPrefix = L"_Render";	break;
	case eOtherProcess:		pwsPrefix = L"_Other";	break;
	case eZygoteProcess:	pwsPrefix = L"_Zygote";	break;
	default:				ATLASSERT(FALSE);		break;
	}
	
	strName.Format(MAIN_PROJECT_NAME L"_clsk12%s_%d_%s",
		pwsPrefix, GetCurrentProcessId(), VERSION_WSTRING_BUILD); 

	return strName;
}

HRESULT CSACModule::Init(eProcessType eType)
{
	CString strName;
	HRESULT hr = eBrowserProcess == eType ? InitLog(LogFileName(strName)) : S_FALSE;
	if(FAILED(hr))
		return hr;

    hr = InitCrashHandler(CrashFileName(eType, strName));
    if (FAILED(hr))
        return hr;

	InitCefDll();

	if(eType != eBrowserProcess)
		return S_OK;

    InitGDIPlus();
   
	return hr;
}

HRESULT CSACModule::Uninit()
{    
	HRESULT hr  = UninitLog();
	ATLASSERT(SUCCEEDED(hr));

	UninitCefDll();
    UninitGDIPlus();

	return S_OK;
}

HRESULT CSACModule::InitCefDll()
{
	return m_hCefDll.Load(L"ACCef.dll");
}

HRESULT CSACModule::UninitCefDll()
{
	if (m_hCefDll)
	{
		m_hCefDll.Free();
	}
	return S_OK;
}

HRESULT CSACModule::InitGDIPlus()
{
    GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

    return S_OK;
}

HRESULT CSACModule::UninitGDIPlus()
{
    Gdiplus::GdiplusShutdown(m_gdiplusToken);

    return S_OK;
}

void CSACModule::SetCefInstance(ICefManager* spMgr) 
{
	m_spCefMgr = spMgr;
}

ICefManager* CSACModule::GetCefMgrInstance()
{
	if (m_spCefMgr)
	{
		return  m_spCefMgr;
	}
	return nullptr;
}

HMODULE CSACModule::GetCefModuleHandle()
{
	if (m_hCefDll)
	{
		return m_hCefDll.Attach(GetModuleHandle(L"ACCef.dll"));
	}
	return nullptr;
}

CLoginWindow* CSACModule::GetLoginWnd()
{
	return m_ploginWnd;
}

bool CSACModule::IsWinLanguageChinese()
{
	LANGID lid = GetSystemDefaultLangID();
	if (lid == 0x0404 || lid == 0x0804 || lid == 0x0c04 || lid == 0x1004)
	{
		return true;
	}

	return false;
}

LPCTSTR CSACModule::GetLanguage()
{
    return g_SACMoudleApp.IsWinLanguageChinese() ? L"cn" : L"en";
}

LPCSTR CSACModule::GetLanguageA()
{
    return g_SACMoudleApp.IsWinLanguageChinese() ? "cn" : "en";
}

int CSACModule::DUIMain(HINSTANCE hInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{        
	LOGPRINTTIMESTAMP(L"ACRun: DUIMain %d", GetTickCount());
	
	CPaintManagerUI::SetInstance(hInstance);

	if (IsWinLanguageChinese())
	{
		CPaintManagerUI::SetDefLang(L"zh_cn");
	}
	else
	{
		CPaintManagerUI::SetDefLang(L"en_us");
	}

	CPaintManagerUI::SetLangXml(L"lang.xml", CPaintManagerUI::GetInstancePath());

	CPaintManagerUI::SetColorXml(CPaintManagerUI::GetInstancePath() + L"\\skin\\color.xml");

//     CPaintManagerUI::SetResourcePath(SkinManager()->GetWindowXMLFolder());
//     CPaintManagerUI::SetResourceImagePath(SkinManager()->GetSkinFolder());
//     
//     CPaintManagerUI::LoadResourceZip(MAKEINTRESOURCE(IDR_SKIN), _T("ZIPRES"));

  //  OnFirstCommandLine(lpstrCmdLine);

	CLoginWindow* mainWindow = new CLoginWindow();
	{
		mainWindow->Create(NULL, L"loginwindow", UI_WNDSTYLE_FRAME, 0L);

		mainWindow->ShowWindow(true);
		mainWindow->CenterWindow();
	}

	m_ploginWnd = mainWindow;

// 	CMainframe* mainWindow = new CMainframe();
// 	{
// 		mainWindow->Create(NULL, L"mainframe", UI_WNDSTYLE_FRAME, 0L);
// 
// 		mainWindow->ShowWindow(true);
// 		mainWindow->CenterWindow();
// 	}

	LOGPRINTTIMESTAMP(L"ACRun: RunMessageLoop %d", GetTickCount());

	BOOL bRet;
	MSG msg = { 0 };
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	LOGPRINTTIMESTAMP(L"ACRun: ExitMessageLoop %d", GetTickCount());
	
	return 0;
}


HRESULT CSACModule::InitLog(LPCWSTR lpwsLogFile)
{
	if (NULL == lpwsLogFile)
		return E_INVALIDARG;

	CString strName;
	

	BOOL bRtn = HELPER::MakeDirExist(lpwsLogFile, true);
	ATLASSERT(bRtn);

	LOGPRINT_INIT_FILE(lpwsLogFile);

	LOGPRINTTIMESTAMP(MAIN_PROJECT_NAME L" Start : %d", GetCurrentProcessId());
	return S_FALSE;
}

HRESULT CSACModule::UninitLog()
{
	LOGPRINTTIMESTAMP(MAIN_PROJECT_NAME L" Stop : %d", GetCurrentProcessId());
	LOGPRINT_UNINIT();
	return S_FALSE;
}

HRESULT CSACModule::InitCrashHandler(LPCWSTR lpwsPrefix)
{
	if(NULL == lpwsPrefix)
		return E_INVALIDARG;

	UINT new_flags =
		SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX;
	UINT existing_flags = SetErrorMode(new_flags);
	SetErrorMode(existing_flags | new_flags);

	CMiniDump::SetDumpFileName(lpwsPrefix, NULL, MAIN_PROJECT_NAME L"Data\\logs");

	bool bRtn = CMiniDump::EnableTrackException(true);
	return bRtn ? S_OK : S_FALSE;
}

HRESULT CSACModule::UninitCrashHandler()
{
	CMiniDump::EnableTrackException(false);
	return S_FALSE;
}

void CSACModule::TerminateChildProcess()
{
	CHandle hSnapShot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	PROCESSENTRY32 pe = { 0 }; 
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapShot, &pe))
		return;

	DWORD dwPIDSelf = GetCurrentProcessId();
	do
	{
		if (pe.th32ProcessID == dwPIDSelf)
			continue;

		if (pe.th32ParentProcessID == dwPIDSelf)
		{
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			if (hProcess)
			{
				::TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
		}
	} while (Process32Next(hSnapShot, &pe));
}

void CSACModule::OnFirstCommandLine(LPCWSTR lpwsCmdLine)
{
	if (NULL == lpwsCmdLine)
		return;
    
// 	CCommandParse parse;
// 	if (!parse.Parse(lpwsCmdLine))
// 		return;
// 
//     CCommand* pCmd = nullptr;
//     
// 	pCmd = parse.FindCommand(L"shellopen");
// 	if (pCmd)
// 	{
// 		OnShellOpen(pCmd, parse);
// 	}
//     pCmd = parse.FindCommand(L"enterclass");
//     if (pCmd)
//     {
//         ParseEnterClassInfo(pCmd->GetArg(0));
//     }
}

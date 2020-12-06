#pragma once
#include <ACCef/ACCef.h>
#include "dllmodule.h"

class CLoginWindow;
class CSACModule
{
public:
	CSACModule(void);
	~CSACModule(void);
private:

	struct ShellOpenCmdLineInfo
	{
		CString strCMD;
		std::map<CString, CString> mapKeyValue;

		bool Find(LPCWSTR lpwsKey, CString& strValue)
		{
			std::map<CString, CString>::iterator it = mapKeyValue.find(lpwsKey);
			if (it != mapKeyValue.end())
			{
				strValue = it->second;
				return true;
			}
			return false;
		}
	};

public:
	HRESULT Init(eProcessType eType);
	HRESULT Uninit();

	HRESULT InitCefDll();
	HRESULT UninitCefDll();

    HRESULT InitGDIPlus();
    HRESULT UninitGDIPlus();

	ICefManager* GetCefMgrInstance();

	HMODULE GetCefModuleHandle();

	CLoginWindow* GetLoginWnd();


public:
	int DUIMain(HINSTANCE hInstance, LPTSTR lpstrCmdLine, int nCmdShow);

	//CString& GetLang();
	void SetCefInstance(ICefManager* spMgr);
	static void TerminateChildProcess();

	//void SendInterfaceTime(DWORD dwCreateTime, DWORD dwLoadTime, LPCWSTR lpurl,BOOL bSucc, __int64 nSize = 0);
	bool IsWinLanguageChinese();
    LPCWSTR GetLanguage();
    LPCSTR GetLanguageA();
	void OnFirstCommandLine(LPCWSTR lpwsCmdLine);

private:
	HRESULT InitLog(LPCWSTR lpwsLogFile);
	HRESULT UninitLog();
	HRESULT InitCrashHandler(LPCWSTR lpwsPrefix);
	HRESULT UninitCrashHandler();
	//HRESULT InitOnlineLog();

private:
	//CString m_strLang;
	CLockCS m_lck;
	eProcessType m_eProcessType;

	CDllModule m_hCefDll;

    //CComPtr<ICefManager> m_spCefMgr;
    //改成普通指针
    ICefManager * m_spCefMgr;

	CLoginWindow* m_ploginWnd;

    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR m_gdiplusToken; 

};

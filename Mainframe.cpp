#include "stdafx.h"
#include "mainframe.h"
#include "UIImageAnimate.h"
#include "CircleImageUI.h"
#include "UIVerticalLayoutBK.h"
#include "DBClickLabelUI.h"
#include "../NetTask/PicTransTask.h"
#include "MsgBox/DuiCloseMessageBox.h"
#include "MsgBox/DuiCommMessageBox.h"
#include "UUID.h"
#include "IniHelper.h"
#include "BrowserTabUI.h"
#include "HelpWebWindow.h"
#include "PathBase.h"
#define TIMER_BY_SECOND                 (WM_USER + 200)
#define TIMER_INTERVAL_BY_SECOND		3000
#define TIMER_DELAYCALL_GETMATERIAL     (WM_USER + 210)
#define TIMER_DELAYCALL_GETCOURSELIST   (WM_USER + 211)
#define TIMER_DELAYCALL_GETCOURSEINFO   (WM_USER + 212)

#define  TimerID_bjh   19867
#define  TimerID_cookiebjh   19868

CMainframe::CMainframe(void)
{
	m_pCloseBtn = NULL;
	m_pBtnExitLogin = NULL;

	m_pWebLoading = NULL;
	m_pWebLoadFail = NULL;
	m_pWebpage = NULL;

	m_pNickName = NULL;

	m_phandImge = NULL;

	proom = nullptr;
}


CMainframe::~CMainframe(void)
{
}

LPCTSTR CMainframe::GetWindowClassName()const
{
	return L"mainframe";
}

void CMainframe::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"closebtn"));

	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"minbtn"));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"maxbtn"));
	m_pRefreshBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"refutbtn"));

	
	m_pBtnMaterialMgr = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"downloadMgrBtn"));
	m_pBtnMateriaAnimate = static_cast<CImageAnimateUI*>(m_PaintManager.FindControl(L"downloadMgrBtnFrame"));


	m_pBtnDevCheck = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"BtnDeviceCheck"));
	m_pBtnExitLogin = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"BtnLoginOut"));
	m_pVersionLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"versionLab"));

	m_pLogoVL = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"logo"));
	m_pNickName = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"Lab_NickName"));
	m_pEditNickName = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"Edit_Nickname"));
	m_pBtnModifyNickName = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"Btn_ModifyNick"));
	m_phandImge = static_cast<CCircleImageUI*>(m_PaintManager.FindControl(L"handimg"));

	m_pCourseWeb = static_cast<CCef3WebUI*>(m_PaintManager.FindControl(L"Cef3WebMyCourseList"));

	SetIcon(IDI_ICON1);
	SetWindowTextW(m_hWnd, App_Name);

	//create new webUI
	m_pCourseWeb->CreateBrowser();
	
	m_pCourseWeb->SetCef3CallBack(this);
}

LRESULT CMainframe::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		return 0;
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

CControlUI* CMainframe::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("CircleImage")) == 0)
	{
		return new CCircleImageUI;
	}
	else if (_tcsicmp(pstrClass, _T("ImageAnimate")) == 0)
	{
		return new CImageAnimateUI;
	}
	else if (_tcsicmp(pstrClass, _T("VerticalLayoutBK")) == 0)
	{
		return new CVerticalLayoutBKUI;
	}
	else if (_tcsicmp(pstrClass, _T("DBCLabel")) == 0)
	{
		return new CDBClickLabelUI;
	}
	else if (_tcsicmp(pstrClass, _T("Cef3Web")) == 0 )
	{
		return new CCef3WebUI;
	}
	else if (_tcsicmp(pstrClass, _T("BrowserTab")) == 0)
	{
		return new CBrowserTabUI;
	}

    return __super::CreateControl(pstrClass);
}

LRESULT CMainframe::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CMainframe::ShowTip(LPCWSTR lpText)
{
	::MessageBox(m_hWnd, lpText, L"提示", MB_OK);
}

const bool CMainframe::Quit(const bool bLogout /*= false*/, const bool bForce /*= false*/)
{
    return true;
}

DuiLib::CDuiString CMainframe::GetSkinFolder()
{
	return L"skin\\";
}

DuiLib::CDuiString CMainframe::GetSkinFile()
{
    return L"mainframe.xml";
}

DuiLib::UILIB_RESOURCETYPE CMainframe::GetResourceType() const
{
	return UILIB_FILE;
}

//void CMainframe::CallJS(LPCWSTR strType, LPCWSTR lpJson)
//{
	//     CUTFString utfJson(lpJson);
	//     string strJson = utfJson.UTF8();
	//     string doc = CBase64::Encode((const unsigned char*)strJson.c_str(), strJson.length());
	//     CUTFString utf(doc.c_str());
	// 
	//     CString strContent;
	//     strContent.Format(L"comm_course_set(\"%s\",\"%s\")", strType, utf.UTF16());
	//     m_pCourseWeb->CallJS(strContent);
//}

void CMainframe::ShowWebLoading()
{

}

void CMainframe::OnWebLoadSucc() 
{

}

void CMainframe::OnWebLoadErr()
{

}

void CMainframe::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pCloseBtn)
		{
			ShowWindow(false);
			PostQuitMessage(0);
		}
		
		else if (msg.pSender == m_pRefreshBtn)
		{
			CalltestJS();
		}
	}
}

void CMainframe::OpenWebPage()
{
	if (m_pCourseWeb)
	{
		//CString  str = L"D:\\Users\\Tan\\source\\repos\\Rocket\\bin\\Debug\\test.html";
		CString  str = L" www.baidu.com";
		m_pCourseWeb->OpenURL(str);
		m_pCourseWeb->OpenURL(L"http://www.baidu.com");
	}
}

DuiLib::CDuiString CMainframe::GetZIPFileName() const
{
	return L"UISkin.zip";
}

void CMainframe::OnBrowserCreated(void* ThisUI)
{
	OpenWebPage();
}

CString MakeSafeJSString(LPCWSTR szStr)
{
	if (!szStr)
		return L"";

	CString arrSpecJSChar[9] =
	{
		L"\\",
		L"'",
		L"\"",
		L"&",
		L"\n",
		L"\r",
		L"\t",
		L"\b",
		L"\f",
	};

	CString arrSpecNewJSChar[9] =
	{
		L"\\\\",
		L"\\'",
		L"\\\"",
		L"\\&",
		L"\\n",
		L"\\r",
		L"\\t",
		L"\\b",
		L"\\f",
	};

	CString strJS = szStr;
	for (int i = 0; i < 9; i++)
	{
		strJS.Replace(arrSpecJSChar[i], arrSpecNewJSChar[i]);
	}
	return strJS;
}

void CMainframe::CalltestJS()
{
	if (m_pCourseWeb) {
		m_pCourseWeb->CallJSCommonFunc(L"123", L"4556");
	}
}

void CMainframe::OnLoadEnd(void* ThisUI)
{
	
}

void CMainframe::OnLoadError(int nError, void* ThisUI)
{
	
}

void CMainframe::OnJSCallCPlus(LPCWSTR lpwsId, LPCWSTR lpwsContent, void* ThisUI)
{
	if (lstrcmpW(lpwsContent, L"enterclass") == 0)
	{
		ShowWindow(false);

		if (proom == nullptr) {
			proom = new CRoomWindow;
			proom->Create(NULL, L"room", UI_WNDSTYLE_FRAME, 0L);
		}

		proom->ShowWindow(true);
		proom->CenterWindow();
	}

	CString str;
	str.Format(L"%s%s", L"这是测试调用  ", lpwsContent);
	ShowTip(str);

	if (lstrcmpW(lpwsId, L"openurl") == 0 && lstrcmpW(lpwsContent, L"") != 0)
	{
		//::ShellExecute(NULL, L"open", lpwsContent, NULL, NULL, SW_SHOWNORMAL);
	}

}

void CMainframe::OnBrowserClosed()
{

}

void CMainframe::OnRenderProcessTerminated(LPCWSTR lpwsUrl, int nStatus)
{
    //课表render进程意外中止,reload
}


void CMainframe::OnTitleChange(LPCWSTR lpTitle, void* ThisUI)
{
	
}

void CMainframe::OnAddressChange(LPCWSTR lpwsaddress, void* ThisUI)
{

}

void CMainframe::OnCookie(LPCWSTR cookie, void* ThisUI)
{
	
}

void CMainframe::OnLoadingStateChange(void* ThisUI, bool bisLoading, bool isgoback, bool isgoforure)
{

}

LRESULT CMainframe::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	return S_OK;
}

LRESULT CMainframe::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    return S_OK;
}
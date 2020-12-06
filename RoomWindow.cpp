#include "stdafx.h"
#include "RoomWindow.h"

CRoomWindow::CRoomWindow()
{
	m_pCloseBtn = NULL;
	m_meUI = NULL;
	m_youUI = NULL;
}

CRoomWindow::~CRoomWindow()
{
}

void CRoomWindow::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"closebtn"));

	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"minbtn"));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"maxbtn"));
	m_pRefreshBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"refutbtn"));

	m_pWBUI = static_cast<CCef3WebUI*>(m_PaintManager.FindControl(L"Cef3WebRoom"));

	m_meUI = static_cast<CVideoUI*>(m_PaintManager.FindControl(_T("my")));
	m_youUI = static_cast<CVideoUI*>(m_PaintManager.FindControl(_T("you")));

	if (m_pWBUI)
	{
		m_pWBUI->SetCef3CallBack(this);
		m_pWBUI->CreateBrowser(BROWSER_BORAD);
	}
}

void CRoomWindow::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pCloseBtn)
		{
			ShowWindow(false);
			PostQuitMessage(0);
		}
		else if (msg.pSender == m_pMinBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (msg.pSender == m_pMaxBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		
	}
}

LRESULT CRoomWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

LRESULT CRoomWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

DuiLib::CControlUI* CRoomWindow::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("Cef3Web")) == 0)
	{
		return new CCef3WebUI;
	}
	return NULL;
}

void CRoomWindow::OnBrowserCreated(void* ThisUI)
{
	if (m_pWBUI)
	{
		CString  str = L"D:\\Users\\Tan\\source\\repos\\Rocket\\bin\\Debug\\skin\\room.html";
		m_pWBUI->OpenURL(str);
		//m_pCourseWeb->OpenURL(L"http://www.baidu.com");
	}
}

void CRoomWindow::OnLoadEnd(void* ThisUI)
{

}

void CRoomWindow::OnLoadError(int nError, void* ThisUI)
{
	
}

void CRoomWindow::OnJSCallCPlus(LPCWSTR lpwsId, LPCWSTR lpwsContent, void* ThisUI)
{

}

DuiLib::UILIB_RESOURCETYPE CRoomWindow::GetResourceType() const
{
	return UILIB_FILE;
}

DuiLib::CDuiString CRoomWindow::GetZIPFileName() const
{
	return L"skin.zip";
}


DuiLib::CDuiString CRoomWindow::GetSkinFile()
{
	return L"room.xml";
}

DuiLib::CDuiString CRoomWindow::GetSkinFolder()
{
	return _T("skin\\");
}






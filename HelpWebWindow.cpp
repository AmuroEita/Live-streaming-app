#include "stdafx.h"
#include "HelpWebWindow.h"

CHelpWebWindow::CHelpWebWindow(IOwnerWndManager* pOwnerWndManager)
    :m_pOwnerWndManager(pOwnerWndManager)
{
	m_pCloseBtn = NULL;
	m_RootVLayoutUI = NULL;

	m_pHorLoading = NULL;
	m_pHorHelpWeb = NULL;
	m_pHorHelpFailPage = NULL;

	m_pHelpWeb = NULL;
}

CHelpWebWindow::~CHelpWebWindow(void)
{
}

LPCTSTR CHelpWebWindow::GetWindowClassName()const
{
	return _T("HelpWebWindow");
}

LRESULT CHelpWebWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (uMsg == WM_NCLBUTTONDBLCLK)
    {
        bHandled = TRUE;
        return 0;
    }

    return 0;
}

void CHelpWebWindow::InitWindow()
{
	m_RootVLayoutUI = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"VRootUI"));
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"closebtn"));

	m_pHorLoading = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"HelpLoading"));
	m_pHorHelpWeb = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"HelppagewebView"));
	m_pHorHelpFailPage = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"HelppageFail"));

	m_pHelpWeb = static_cast<CCef3WebUI*>(m_PaintManager.FindControl(L"HelppageWeb"));
	if (m_pHelpWeb)
	{
		m_pHelpWeb->SetCef3CallBack(this);
		m_pHelpWeb->CreateBrowser();
	}
}

void CHelpWebWindow::SetURL(LPCWSTR url)
{
	m_strurl = url;
}

DuiLib::CDuiString CHelpWebWindow::GetSkinFolder()
{
	//return SkinManager()->GetWindowXMLFolder();
	return L"skin\\";
}

DuiLib::CDuiString CHelpWebWindow::GetSkinFile()
{
    //return (g_SACMoudleApp.IsWinLanguageChinese() ? L"helpWebWindow.xml" : L"helpWebWindow-en.xml");
    return L"helpWebWindow.xml";
}

DuiLib::UILIB_RESOURCETYPE CHelpWebWindow::GetResourceType() const
{
	return UILIB_FILE;
}

void CHelpWebWindow::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pCloseBtn)
		{
            SafeClose(NULL);
		}
	}
}

void CHelpWebWindow::DoClose()
{
    if (m_pOwnerWndManager)
        m_pOwnerWndManager->ObserverWndDestroy(this);
}

void CHelpWebWindow::OpenPage()
{
	if (m_pHelpWeb)
	{
		m_pHelpWeb->OpenURL(m_strurl);
	}
}

DuiLib::CDuiString CHelpWebWindow::GetZIPFileName() const
{
	return L"UISkin.zip";
}

DuiLib::CControlUI* CHelpWebWindow::CreateControl(LPCTSTR pstrClass)
{
    return __super::CreateControl(pstrClass);
}

void CHelpWebWindow::OnBrowserCreated(void* ThisUI)
{
	if (ThisUI == m_pHelpWeb)
	{
		OpenPage();
	}
}

void CHelpWebWindow::OnLoadEnd(void* ThisUI)
{
	// 隐藏loading
	//show window

	if (m_pHorHelpFailPage)
	{
		m_pHorHelpFailPage->SetVisible(false);
	}

	if (m_pHorLoading)
	{
		m_pHorLoading->SetVisible(false);
	}

	if (m_pHorHelpWeb)
	{
		m_pHorHelpWeb->SetVisible(true);
	}
}

void CHelpWebWindow::OnLoadError(int nError, void* ThisUI)
{
	if (m_pHorHelpWeb)
	{
		m_pHorHelpWeb->SetVisible(false);
	}

	if (m_pHorLoading)
	{
		m_pHorLoading->SetVisible(false);
	}

	if (m_pHorHelpFailPage)
	{
		m_pHorHelpFailPage->SetVisible(true);
	}
}

void CHelpWebWindow::OnJSCallCPlus(LPCWSTR lpwsId, LPCWSTR lpwsContent, void* ThisUI)
{
	if (lstrcmpiW(lpwsId, L"helpbtnclick") == 0)
	{
		if (lstrcmpiW(lpwsContent, L"cancel") == 0 )
		{
            SafeClose(NULL);
		}
		else if (lstrcmpiW(lpwsContent, L"ok") == 0)
		{
            SafeClose(NULL);
		}
	}
}
void CHelpWebWindow::OnBrowserClosed()
{

}

void CHelpWebWindow::OnRenderProcessTerminated(LPCWSTR lpwsUrl, int nStatus)
{
    //todo render进程意外中止
}
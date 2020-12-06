#include "stdafx.h"
#include "DuiCommMessageBox.h"
#include "DuiCloseMessageBox.h"

Dui_Return_Type DuiCommMsgBox(HWND hwnd, LPCWSTR lpXmlPath, const int btnType, LPCWSTR lpTitle, LPCWSTR lpText, LPCWSTR lpOk, LPCWSTR lpCancel)
{
    CDuiCommMessageBox closeMB(lpXmlPath);
	closeMB.Create(hwnd, L"", UI_WNDSTYLE_FRAME, WS_EX_TOOLWINDOW);
    closeMB.SetText(btnType, lpTitle, lpText, lpOk, lpCancel);

	if(::IsIconic(hwnd))
	{
		::SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
    ::SetForegroundWindow(hwnd);
    ::SetActiveWindow(hwnd);
    ::BringWindowToTop(hwnd);
	closeMB.CenterWindow();

    Dui_Return_Type res = (Dui_Return_Type)closeMB.ShowModal();
	return res;
}

CDuiCommMessageBox::CDuiCommMessageBox(LPCWSTR lpXmlPath)
: m_strXmlPath(lpXmlPath)
, m_pTitleLab(NULL)
, m_pMsgText(NULL)
, m_pCloseBtn(NULL)
, m_pOKBtn(NULL)
, m_pCancelBtn(NULL)
{
}

CDuiCommMessageBox::~CDuiCommMessageBox()
{
}

void CDuiCommMessageBox::Notify( TNotifyUI& msg )
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pCloseBtn)
		{
			Close(DUI_MBCLOSE);
		}
		else if ( msg.pSender == m_pOKBtn)
		{
			Close(DUI_MBOK);
		}
        else if (msg.pSender == m_pCancelBtn)
        {
            Close(DUI_MBCANCEL);
        }
	}
}

LPCTSTR CDuiCommMessageBox::GetWindowClassName() const
{
	return L"DuiCommMessageBoxWnd";
}

DuiLib::CDuiString CDuiCommMessageBox::GetSkinFolder()
{
	return _T("UISkin\\");
}

DuiLib::CDuiString CDuiCommMessageBox::GetSkinFile()
{
	return m_strXmlPath.IsEmpty() ? L"DuiCommMessageBox.xml" : m_strXmlPath;
}

DuiLib::UILIB_RESOURCETYPE CDuiCommMessageBox::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

DuiLib::CDuiString CDuiCommMessageBox::GetZIPFileName() const
{
	return L"UISkin.zip";
}

void CDuiCommMessageBox::InitWindow()
{
    m_pTitleLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"titleLab"));
    m_pMsgText = static_cast<CTextUI*>(m_PaintManager.FindControl(L"msgText"));
    m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"closeBtn"));
    m_pOKBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"okBtn"));
    m_pCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"cancelBtn"));
}

LRESULT CDuiCommMessageBox::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    if (uMsg == WM_NCLBUTTONDBLCLK)
    {
        bHandled = TRUE;
        return 0;
    }
    return 0;
}

void CDuiCommMessageBox::SetText(const int btnType, LPCWSTR lpTitle, LPCWSTR lpText, LPCWSTR lpOk, LPCWSTR lpCancel)
{
    bool bShowOkBtn = (btnType & DUI_MBOK) == DUI_MBOK;
    bool bShowCancelBtn = (btnType & DUI_MBCANCEL) == DUI_MBCANCEL;
    bool bShowCloseBtn = (btnType & DUI_MBCLOSE) == DUI_MBCLOSE;

    if (bShowOkBtn)
        utils::SetVisible(m_pOKBtn, true);

    if (bShowCancelBtn)
        utils::SetVisible(m_pCancelBtn, true);

    if(bShowCloseBtn)
        utils::SetVisible(m_pCloseBtn, true);

    //如果只显示一个按钮，去掉padding
    if (bShowOkBtn != bShowCancelBtn)
    {
        RECT rcPadding = { 0,0,0,0 };
        utils::SetPadding(m_pOKBtn, rcPadding);
        utils::SetPadding(m_pCancelBtn, rcPadding);
    }

    if(lpTitle)
        utils::SetText(m_pTitleLab, lpTitle);

    if(lpText)
        utils::SetText(m_pMsgText, lpText);

    if(lpOk)
        utils::SetText(m_pOKBtn, lpOk);

    if(lpCancel)
        utils::SetText(m_pCancelBtn, lpCancel);
}

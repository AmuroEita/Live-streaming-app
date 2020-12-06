#include "stdafx.h"
#include "DuiCloseMessageBox.h"

Dui_Return_Type DuiMsgBox(HWND hwnd, const wchar_t * pwszMsg, const wchar_t * pwszCaption /*= NULL*/, const int btnType /*= DUI_MBOK*/)
{
    if (pwszMsg == NULL || wcslen(pwszMsg) <= 0)
        return DUI_MBCLOSE;

	static bool bCreate = false;
	if(bCreate)
		return DUI_MBCLOSE;

	CDuiCloseMessageBox closeMB;
	closeMB.Create(hwnd, L"", WS_VISIBLE|WS_POPUP, WS_EX_TOOLWINDOW|WS_EX_LAYERED);
	if(::IsIconic(hwnd))
	{
		::SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
    ::SetForegroundWindow(hwnd);
    ::SetActiveWindow(hwnd);
    ::BringWindowToTop(hwnd);
	closeMB.SetMessage(pwszMsg, pwszCaption, btnType, DUI_ICON_CONFIRM);
	closeMB.CenterWindow();
	bCreate = true;
	Dui_Return_Type res = (Dui_Return_Type)closeMB.ShowModal();
	bCreate = false;
	return res;
}

CDuiCloseMessageBox::CDuiCloseMessageBox()
: m_pLabelCaption(NULL)
, m_pTextMsgText(NULL)
, m_pBtnClose(NULL)
, m_pBtnOK(NULL)
, m_pBtnCancel(NULL)
, m_pBtnContainer(NULL)
, m_rcCancelBtnPadding()
{
}

CDuiCloseMessageBox::~CDuiCloseMessageBox()
{
}

void CDuiCloseMessageBox::Notify( TNotifyUI& msg )
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pBtnClose)
		{
			Close(DUI_MBCLOSE);
		}
		else if ( msg.pSender == m_pBtnOK )
		{
			Close(DUI_MBOK);
		}
        else if (msg.pSender == m_pBtnCancel)
        {
            Close(DUI_MBCANCEL);
        }
	}
}

LPCTSTR CDuiCloseMessageBox::GetWindowClassName() const
{
	return L"MessageBoxWnd";
}

DuiLib::CDuiString CDuiCloseMessageBox::GetSkinFile()
{
	return L"DuiCloseMessageBox.xml";
}

DuiLib::UILIB_RESOURCETYPE CDuiCloseMessageBox::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

DuiLib::CDuiString CDuiCloseMessageBox::GetZIPFileName() const
{
	return L"UISkin.zip";
}

void CDuiCloseMessageBox::InitWindow()
{
    m_pLabelCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"caption"));
    m_pTextMsgText = static_cast<CTextUI*>(m_PaintManager.FindControl(L"msgtext"));
	m_pBtnClose = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"closebtn"));
	m_pBtnOK = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnOK"));
    m_pBtnCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnCancel"));
    m_pBtnContainer = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"btnContainer"));
	m_pLabelIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"icon"));
	m_pGap = static_cast<CControlUI*>(m_PaintManager.FindControl(L"gap"));
}

LRESULT CDuiCloseMessageBox::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    if (uMsg == WM_NCLBUTTONDBLCLK)
    {
        bHandled = TRUE;
        return 0;
    }
    return 0;
}

void CDuiCloseMessageBox::SetMessage(const wchar_t * pwszMsg, const wchar_t * pwszCaption, const int btnType, const int msgicon)
{
    if (m_pLabelCaption != NULL && pwszCaption != NULL)
        m_pLabelCaption->SetText(pwszCaption);

	int nwidth = 0;
	if (m_pLabelIcon)
	{
		CDuiString strIcon;
		if (msgicon == DUI_ICON_INFO)strIcon = L"MessageBox\\icon_info.png";
		else if (msgicon == DUI_ICON_ERROR)strIcon = L"MessageBox\\icon_error.png";
		else if (msgicon == DUI_ICON_CONFIRM)strIcon = L"MessageBox\\icon_confirm.png";
			
		TImageInfo* ptinfo = CRenderEngine::LoadImage(strIcon.GetData());
		if (ptinfo)
		{
			nwidth += ptinfo->nX;
			m_pLabelIcon->SetFixedWidth(ptinfo->nX);
			m_pLabelIcon->SetFixedHeight(ptinfo->nY);
			if (ptinfo->hBitmap)
			{
				DeleteObject(ptinfo->hBitmap);
			}
			delete ptinfo;
		}
		m_pLabelIcon->SetBkImage(strIcon);
	}
	RECT rcText = { 0 };

	CDuiString strText;
	m_PaintManager.GetLangText(pwszMsg, strText);

	if (strText.IsEmpty())
	{
		strText = pwszMsg;
	}

	CRenderEngine::DrawText(m_PaintManager.GetPaintDC(), &m_PaintManager, rcText, strText, 0, 0, DT_CALCRECT |DT_VCENTER);
	nwidth += min(160,max(80,rcText.right));
    if (m_pTextMsgText != NULL && pwszMsg != NULL)
        m_pTextMsgText->SetText(pwszMsg);

	SetWindowPos(m_hWnd, NULL, 0, 0, nwidth + 120, 245, SWP_NOMOVE);

    m_pBtnOK->SetVisible(false);
    m_pBtnCancel->SetVisible(false);
   // m_pBtnCancel->SetPadding(m_rcCancelBtnPadding);

    int iButtonCount = 0;
    if ((btnType & DUI_MBOK) == DUI_MBOK)
    {
        m_pBtnOK->SetVisible(true);
        ++iButtonCount;
    }

    if ((btnType & DUI_MBCANCEL) == DUI_MBCANCEL)
    {
        m_pBtnCancel->SetVisible(true);
        ++iButtonCount;
    }

    if (iButtonCount == 1)
    {
		m_pGap->SetVisible(false);
    }
	m_PaintManager.NeedUpdate();

	CRect rc(0, 0, nwidth + 60, 190);
	InvalidateRect(m_hWnd, rc, TRUE);
}

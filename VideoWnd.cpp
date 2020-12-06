#include "stdafx.h"
#include "VideoWnd.h"
#include "Utils.h"

UINT CVideoWnd::s_uMsgStateChanged = RegisterWindowMessage(L"VideoWndStateChanged_RegiterMessageString");


CVideoWnd::CVideoWnd()
{
	m_hParentWnd=NULL;
	m_nNotifyEnterMsgId=0;
	m_nNotifyLeaveMsgId=0;
	m_nNotifyLButtonDownMsgId = 0;
	m_bMouseTracking=FALSE;
	m_hNotifyWnd = NULL;

	m_pRootVL = NULL;
	m_pImgHL = NULL;
	m_pTitleImg = NULL;
	m_pTileLab = NULL;

	m_pUILabelDefaultBkImage = NULL;
	m_pHorUI = NULL;
}

CVideoWnd::~CVideoWnd()
{

}

void CVideoWnd::SetBkImage(LPCTSTR lpszBkImage)
{
	utils::SetBkImage(m_pRootVL, lpszBkImage);
}

void CVideoWnd::SetBkColor(DWORD dwBkColor)
{
	utils::SetBkColor(m_pRootVL, dwBkColor);
}

void CVideoWnd::SetBkText(LPCTSTR lpszText)
{
	utils::SetText(m_pTileLab, lpszText);
	if(lpszText && lstrlen(lpszText))
		utils::SetVisible(m_pTileLab, true);
	else
		utils::SetVisible(m_pTileLab, false);
}

void CVideoWnd::SetBkIcon(LPCTSTR lpszBkIcon)
{
	if (!lpszBkIcon)
	{
		utils::SetVisible(m_pTitleImg, false);
		return;
	}

	const TImageInfo* data = m_PaintManager.GetImageEx((LPCTSTR)lpszBkIcon, NULL, 0);
	if (data && m_pTitleImg && m_pImgHL)
	{
		m_pTitleImg->SetVisible(true);
		m_pTitleImg->SetFixedWidth(data->nX);
		m_pTitleImg->SetFixedHeight(data->nY);
		m_pImgHL->SetFixedHeight(data->nY);
		m_pTitleImg->SetBkImage(lpszBkIcon);
	}
}

LPCTSTR CVideoWnd::GetWindowClassName() const
{
	return _T("VideoWnd");
}


void CVideoWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
}

void CVideoWnd::InitWindow()
{
	m_pRootVL = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"VRootUI"));
	m_pImgHL = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"imgHL"));
	m_pTitleImg = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"titleImg"));
	m_pTileLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"titleLab"));

	m_pUILabelDefaultBkImage = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"DefaultImageBk"));

	m_pHorUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"imgHLLLLL"));
}

LRESULT CVideoWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			if(m_hParentWnd!=NULL && m_nNotifyLButtonDownMsgId!=0)
				::PostMessage(m_hParentWnd,m_nNotifyLButtonDownMsgId,wParam,lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			if(m_hParentWnd)
			{
				::PostMessage(m_hParentWnd, WM_RBUTTONDOWN, wParam, lParam);
			}
		}
		break;
	case WM_WINDOWPOSCHANGED:
		{
		
		}
		break;
	default:
		bHandled=FALSE;
		break;
	}

	bHandled = FALSE;
	return 0;
}

void CVideoWnd::RefreshWnd()
{
	if(IsWindow(m_hWnd))
	{
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

DuiLib::UILIB_RESOURCETYPE CVideoWnd::GetResourceType() const
{
	return UILIB_FILE;
}

DuiLib::CDuiString CVideoWnd::GetSkinFile()
{
	return _T("VideoWnd.xml");
}

DuiLib::CDuiString CVideoWnd::GetSkinFolder()
{
	return _T("BMSkin\\");
}

void CVideoWnd::SetDefaultCtrolBkImage(LPCTSTR lpszBkImage)
{
	if (m_pUILabelDefaultBkImage)
	{
		m_pUILabelDefaultBkImage->SetBkImage(lpszBkImage);
	}
}

void CVideoWnd::SetImageHorheiandwidth(int iheight, int iwidth)
{
	if (m_pUILabelDefaultBkImage && m_pHorUI )
	{
		m_pUILabelDefaultBkImage->SetFixedWidth(iwidth);
		m_pHorUI->SetFixedHeight(iheight);
	}
}

#include "stdafx.h"
#include "BrowserTabUI.h"


CBrowserTabUI::CBrowserTabUI()
{
}


CBrowserTabUI::~CBrowserTabUI()
{
}

LPCTSTR CBrowserTabUI::GetClass() const
{
	return L"BrowserTabUI";
}

LPVOID CBrowserTabUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("BrowserTabUI")) == 0)
		return static_cast<CBrowserTabUI*>(this);

	return COptionUI::GetInterface(pstrName);
}

void CBrowserTabUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
		if (m_pParent != NULL) m_pParent->DoEvent(event);
		else COptionUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_BUTTONDOWN )
	{
		if (::PtInRect(&m_rcClose, event.ptMouse)) {
			Invalidate();
		}
	}
	if (event.Type == UIEVENT_MOUSEMOVE)
	{
		if (::PtInRect(&m_rcClose, event.ptMouse))
		{
			if ((m_uCloseButtonState & UISTATE_HOT) == 0)
			{
				m_uCloseButtonState |= UISTATE_HOT;
				Invalidate();
			}	
		}
		else
		{
			if ((m_uCloseButtonState & UISTATE_HOT) != 0) {
				m_uCloseButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
		}
	}
	if (event.Type == UIEVENT_BUTTONUP)
	{
		if (::PtInRect(&m_rcClose, event.ptMouse))
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_BROWSERBAR_CLOSETAB, event.wParam, event.lParam);
		}
	}

	COptionUI::DoEvent(event);
}

void CBrowserTabUI::DoInit()
{
	m_uCloseButtonState = 0;
}

void CBrowserTabUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	COptionUI::DoPaint(hDC, rcPaint);
	PaintIcon(hDC, rcPaint);
	PaintClose(hDC, rcPaint);
}

void CBrowserTabUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
	return;
}

void CBrowserTabUI::PaintStatusImage(HDC hDC)
{
	COptionUI::PaintStatusImage(hDC);
}

void CBrowserTabUI::PaintIcon(HDC hDC, const RECT& rcpaint)
{
	RECT rc;
	rc.left = m_rcItem.left + m_rcIconPadding.left;
	rc.top = m_rcItem.top + m_rcIconPadding.top;
	rc.right = rc.left + m_szIcon.cx;
	rc.bottom = rc.top + m_szIcon.cy;

	CRenderEngine::DrawImageString(hDC, m_pManager, rc, m_rcPaint, m_sIconImage, L"");
}

void CBrowserTabUI::PaintClose(HDC hDC, const RECT& rcpaint)
{
	RECT rc;
	rc.right = m_rcItem.right - m_rcClosePadding.right;
	rc.top = m_rcItem.top + m_rcClosePadding.top;
	rc.left = rc.right - m_szClose.cx;
	rc.bottom = rc.top + m_szClose.cy;

	m_rcClose = rc;

	if ((m_uCloseButtonState & UISTATE_HOT) != 0) {
		CRenderEngine::DrawImageString(hDC, m_pManager, rc, m_rcPaint, m_sCloseHotImge, L"");
	}
	else
	{
		CRenderEngine::DrawImageString(hDC, m_pManager, rc, m_rcPaint, m_sCloseImage, L"");
	}
}

void CBrowserTabUI::SetIconSize(SIZE szIcon)
{
	m_szIcon = szIcon;
}

void CBrowserTabUI::SetIconPadding(RECT rcPadding)
{
	m_rcIconPadding = rcPadding;
}

void CBrowserTabUI::SetIconImage(LPCTSTR pStrCursor)
{
	m_sIconImage = pStrCursor;
}

void CBrowserTabUI::SetCloseSize(SIZE szIcon)
{
	m_szClose = szIcon;
}

void CBrowserTabUI::SetClosePadding(RECT rcPadding)
{
	m_rcClosePadding = rcPadding;
}

void CBrowserTabUI::SetCloseImage(LPCTSTR pStrCursor)
{
	m_sCloseImage = pStrCursor;
}

void CBrowserTabUI::SetCloseHotImage(LPCTSTR pStrCloseHotImage)
{
	m_sCloseHotImge = pStrCloseHotImage;
}

void CBrowserTabUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("iconImage")) == 0)
	{
		SetIconImage(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("closeImage")) == 0) 
	{
		SetCloseImage(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("closehotImage")) == 0)
	{
		SetCloseHotImage(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("closePadding")) == 0)
	{
		RECT rcTextPadding = { 0 };
		LPTSTR pstr = NULL;
		rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
		rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		SetClosePadding(rcTextPadding);
	}
	else if (_tcscmp(pstrName, _T("iconPadding")) == 0)
	{
		RECT rcTextPadding = { 0 };
		LPTSTR pstr = NULL;
		rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
		rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		SetIconPadding(rcTextPadding);
	}
	else if (_tcscmp(pstrName, _T("closesize")) == 0)
	{
		SIZE rcs = {0};
		LPTSTR pstr = NULL;
		rcs.cx = _tcstol(pstrValue, &pstr, 10);	ASSERT(pstr);
		rcs.cy = _tcstol(pstr + 1, &pstr, 10);		ASSERT(pstr);

		SetCloseSize(rcs);
	}
	else if (_tcscmp(pstrName, _T("iconsize")) == 0)
	{
		SIZE rcs = { 0 };
		LPTSTR pstr = NULL;
		rcs.cx = _tcstol(pstrValue, &pstr, 10);	ASSERT(pstr);
		rcs.cy = _tcstol(pstr + 1, &pstr, 10);		ASSERT(pstr);

		SetIconSize(rcs);
	}
	else
	{
		COptionUI::SetAttribute(pstrName, pstrValue);
	}
}

bool CBrowserTabUI::TestClose(POINT ptMouse)
{
	return true;
}

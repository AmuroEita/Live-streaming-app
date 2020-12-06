#include "stdafx.h"
#include "UIListItemCust.h"


CUIListItemCust::CUIListItemCust()
{
	
}


LPCTSTR CUIListItemCust::GetClass() const
{
	return _T("UIListItemCust");
}

LPVOID CUIListItemCust::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, L"ListItemWeijian") == 0) return static_cast<CUIListItemCust*>(this);
	return CListLabelElementUI::GetInterface(pstrName);
}

void CUIListItemCust::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("iconimage")) == 0)
	{
		SetIconImge(pstrValue);
	}
	if (_tcsicmp(pstrName, _T("imagepath")) == 0) {
		TImageInfo * pInfo = CRenderEngine::LoadImage(pstrValue);
		if (pInfo)
		{
			if (m_pImagePathBitmap)
			{
				delete m_pImagePathBitmap;
				m_pImagePathBitmap = NULL;
			}
			m_pImagePathBitmap = Gdiplus::Bitmap::FromHBITMAP(pInfo->hBitmap, NULL);
			if (pInfo->hBitmap)
			{
				DeleteObject(pInfo->hBitmap);
			}
			delete pInfo;
		}
		Invalidate();
	}
	else
		CListLabelElementUI::SetAttribute(pstrName, pstrValue);
}

// void CUIListItemCust::PaintBkImage(HDC hDC)
// {
// 	CListLabelElementUI::PaintBkImage(hDC);
// 	Gdiplus::Graphics graphics(hDC);
// 	if (m_pImagePathBitmap)
// 	{
// 		Gdiplus::TextureBrush tBrush2(m_pImagePathBitmap);
// 		Gdiplus::REAL cx = (Gdiplus::REAL)(m_rcItem.right - m_rcItem.left);
// 		Gdiplus::REAL cy = (Gdiplus::REAL)(m_rcItem.bottom - m_rcItem.top);
// 		Gdiplus::RectF rc((Gdiplus::REAL)m_rcItem.left, (Gdiplus::REAL)m_rcItem.top, cx, cy);
// 		graphics.SetClip(rc);
// 		tBrush2.SetWrapMode(Gdiplus::WrapModeTile);
// 		graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
// 		tBrush2.TranslateTransform(rc.GetLeft(), rc.GetTop());
// 		tBrush2.ScaleTransform(cx / m_pImagePathBitmap->GetWidth(), cy / m_pImagePathBitmap->GetHeight());
// 		graphics.FillEllipse(&tBrush2, rc);
// 	}
// }

void CUIListItemCust::DrawItemText(HDC hDC, const RECT& rcItem)
{
	RECT rc;
	rc.left = m_rcItem.left + 20;
	rc.top = m_rcItem.top + 5;
	rc.right = rc.left + 35;
	rc.bottom = rc.top + 35;

	CRenderEngine::DrawImageString(hDC, m_pManager, rc, m_rcPaint, m_sSelIconimge, L"");

	CDuiString sText = GetText();
	if (sText.IsEmpty()) return;

	if (m_pOwner == NULL) return;
	TListInfoUI* pInfo = m_pOwner->GetListInfo();
	DWORD iTextColor = pInfo->dwTextColor;
	if ((m_uButtonState & UISTATE_HOT) != 0) {
		iTextColor = pInfo->dwHotTextColor;
	}
	if (IsSelected()) {
		iTextColor = pInfo->dwSelectedTextColor;
		if (!m_sSelText.IsEmpty()) sText = m_sSelText;
	}
	if (!IsEnabled()) {
		iTextColor = pInfo->dwDisabledTextColor;
	}
	int nLinks = 0;
	RECT rcText = rcItem;
	rcText.left += pInfo->rcTextPadding.left;
	rcText.right -= pInfo->rcTextPadding.right;
	rcText.top += pInfo->rcTextPadding.top;
	rcText.bottom -= pInfo->rcTextPadding.bottom;

	rcText.left += rc.right + 18;

	if (pInfo->bShowHtml)
		CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, sText, iTextColor, \
			NULL, NULL, nLinks, DT_SINGLELINE | pInfo->uTextStyle);
	else
		CRenderEngine::DrawText(hDC, m_pManager, rcText, sText, iTextColor, \
			pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
}
// 
// void CUIListItemCust::PaintBkImage(HDC hDC)
// {
// 	CListLabelElementUI::PaintBkImage(hDC);
// 
// 	//draw icon
// 	RECT rc;
// 	rc.left = m_rcItem.left + 20;
// 	rc.top = m_rcItem.top;
// 	rc.right = rc.left + 35;
// 	rc.bottom = rc.top + 35;
// 	//	CRenderEngine::DrawImageString(hDC, m_pManager, rc, m_rcPaint, m_sSelIconimge, L"");
// 
// 	Gdiplus::Graphics graphics(hDC);
// 	if (m_pImagePathBitmap)
// 	{
// 		Gdiplus::TextureBrush tBrush2(m_pImagePathBitmap);
// 		// 		Gdiplus::REAL cx = (Gdiplus::REAL)(m_rcItem.right - m_rcItem.left);
// 		// 		Gdiplus::REAL cy = (Gdiplus::REAL)(m_rcItem.bottom - m_rcItem.top);
// 		// 		Gdiplus::RectF rc((Gdiplus::REAL)m_rcItem.left, (Gdiplus::REAL)m_rcItem.top, cx, cy);
// 		// 		
// 		Gdiplus::REAL cx = (Gdiplus::REAL)(rc.right - rc.left);
// 		Gdiplus::REAL cy = (Gdiplus::REAL)(rc.bottom - rc.top);
// 		Gdiplus::RectF rc((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top, cx, cy);
// 
// 		graphics.SetClip(rc);
// 		tBrush2.SetWrapMode(Gdiplus::WrapModeTile);
// 		graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
// 		tBrush2.TranslateTransform(rc.GetLeft(), rc.GetTop());
// 		tBrush2.ScaleTransform(cx / m_pImagePathBitmap->GetWidth(), cy / m_pImagePathBitmap->GetHeight());
// 		graphics.FillEllipse(&tBrush2, rc);
// 	}
// }

LPCTSTR CUIListItemCust::GetIconImge()
{
	return m_sSelIconimge;
}

void CUIListItemCust::SetIconImge(LPCTSTR pstrIcon)
{
	m_sSelIconimge = pstrIcon;
}

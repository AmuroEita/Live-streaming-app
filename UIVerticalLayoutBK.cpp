#include "stdafx.h"
#include "UIVerticalLayoutBK.h"

CVerticalLayoutBKUI::CVerticalLayoutBKUI(void)
{
}

CVerticalLayoutBKUI::~CVerticalLayoutBKUI(void)
{
}

LPCTSTR CVerticalLayoutBKUI::GetClass() const
{
	return _T("VerticalLayoutBKUI");
}

LPVOID CVerticalLayoutBKUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("VerticalLayoutBKUI")) == 0)
		return static_cast<CVerticalLayoutBKUI*>(this);

	return CVerticalLayoutUI::GetInterface(pstrName);
}

void CVerticalLayoutBKUI::PaintBkImage(HDC hDC)
{
	if (m_sBkImage.IsEmpty()) return;
	const TImageInfo* data = NULL;

	data = m_pManager->GetImageEx((LPCTSTR)m_sBkImage, (LPCTSTR)NULL, 0);
	
	if (!data) return;
	RECT rcSource = {0};
	double fScale = (m_rcItem.right - m_rcItem.left)*1.0 / (m_rcItem.bottom - m_rcItem.top);
	if (data->nX*1.0/data->nY >fScale)
	{
		rcSource.bottom = data->nY;
		rcSource.right = (long)(data->nY*fScale);
	}
	else 
	{
		rcSource.right = data->nX;
		rcSource.bottom = data->nY;
		rcSource.top = (long)(rcSource.bottom - data->nX / fScale);
	}
	RECT rcCorner = { 0,0,0,0 };
	CRenderEngine::DrawImage(hDC, data->hBitmap, m_rcItem, m_rcPaint, rcSource, rcCorner, false);
}

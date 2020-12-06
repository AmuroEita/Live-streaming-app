#include "StdAfx.h"
#include "CircleImageUI.h"

#include <atlconv.h>
#include <Shlwapi.h>

CCircleImageUI::CCircleImageUI()
{
	m_pImagePathBitmap = NULL;
}

CCircleImageUI::~CCircleImageUI()
{
	if (m_pImagePathBitmap)
	{
		delete m_pImagePathBitmap;
		m_pImagePathBitmap = NULL;
	}
}

LPCTSTR CCircleImageUI::GetClass() const
{
	return _T("CircleImageUI");
}

LPVOID CCircleImageUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("CircleImage")) == 0) return static_cast<CCircleImageUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

void CCircleImageUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("imagepath")) == 0 ) {
		TImageInfo * pInfo = CRenderEngine::LoadImage(pstrValue);
		if (pInfo)
		{
			
			if (m_pImagePathBitmap)
			{
				delete m_pImagePathBitmap;
				m_pImagePathBitmap = NULL;
			}
			m_pImagePathBitmap = Gdiplus::Bitmap::FromHBITMAP(pInfo->hBitmap,NULL);
			if (pInfo->hBitmap)
			{
				DeleteObject(pInfo->hBitmap);
			}
			delete pInfo;
		}
		Invalidate();
	}
	else
		CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CCircleImageUI::PaintBkImage(HDC hDC)
{
	CLabelUI::PaintBkImage(hDC);
	Gdiplus::Graphics graphics(hDC);
	if (m_pImagePathBitmap)
    { 
		Gdiplus::TextureBrush tBrush2(m_pImagePathBitmap);
        Gdiplus::REAL cx = (Gdiplus::REAL)(m_rcItem.right - m_rcItem.left);
        Gdiplus::REAL cy = (Gdiplus::REAL)(m_rcItem.bottom - m_rcItem.top);
		Gdiplus::RectF rc((Gdiplus::REAL)m_rcItem.left, (Gdiplus::REAL)m_rcItem.top, cx, cy);
		graphics.SetClip(rc);
		tBrush2.SetWrapMode(Gdiplus::WrapModeTile);
		graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
        tBrush2.TranslateTransform(rc.GetLeft(), rc.GetTop());
        tBrush2.ScaleTransform(cx / m_pImagePathBitmap->GetWidth(), cy / m_pImagePathBitmap->GetHeight());
		graphics.FillEllipse(&tBrush2, rc);
	}
}
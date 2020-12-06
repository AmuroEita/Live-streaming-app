#include "StdAfx.h"
#include "UIImageAnimate.h"

CImageAnimateUI::CImageAnimateUI()
{
	m_nFrameIndex = 0;
	m_nFrameCount = 1;
	m_nFrameTime = 1;
	m_bRepeatMode = FALSE;
}

CImageAnimateUI::~CImageAnimateUI()
{

}

void CImageAnimateUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("framecount")) == 0 ) {
		m_nFrameCount = _ttoi(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("RepeatMode")) == 0 ) {
		m_bRepeatMode = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("frametime")) == 0 ) {
		m_nFrameTime = _ttoi(pstrValue);
	}
	else
		CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CImageAnimateUI::PaintBkImage(HDC hDC)
{
	if( m_sBkImage.IsEmpty() ) return;

	const TImageInfo* data = m_pManager->GetImageEx((LPCTSTR)m_sBkImage, NULL, 0);
	if(data)
	{
		int nFrameWidth = data->nX/m_nFrameCount;

		CString strSrc;
		strSrc.Format(L"%d,%d,%d,%d", nFrameWidth*m_nFrameIndex,0,nFrameWidth*(m_nFrameIndex+1),data->nY);

		CString sImg = _T("file='");
		sImg += m_sBkImage;
		sImg += L"' source='";
		sImg += strSrc;
		sImg += L"'";
		DrawImage(hDC, (LPCTSTR)sImg);
	}
}

void CImageAnimateUI::Start()
{
	m_nFrameIndex = 0;
	Invalidate();

	m_pManager->SetTimer(this, ANIMATE_TIMERID, m_nFrameTime);
}

void CImageAnimateUI::Stop()
{
	m_pManager->KillTimer(this, ANIMATE_TIMERID);
}

void CImageAnimateUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_TIMER )
		OnTimer( (UINT_PTR)event.wParam );
	CButtonUI::DoEvent(event);
}

void CImageAnimateUI::OnTimer( UINT_PTR idEvent )
{
	if( ANIMATE_TIMERID == idEvent )
	{
		m_nFrameIndex++;
		if( m_nFrameIndex >= m_nFrameCount )
		{
			m_nFrameIndex = 0;
			if( !m_bRepeatMode )
			{
				m_pManager->KillTimer(this, ANIMATE_TIMERID);
				m_pManager->SendNotify(this, DUI_NOTIFY_ANIMATEFINISH);
			}
		}
		Invalidate();
	}
}
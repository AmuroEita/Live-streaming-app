#include "stdafx.h"
#include "UIVideo.h"
namespace DuiLib
{

#define HIDE_HANDUP_TIMERID  1000

	CVideoUI::CVideoUI() 
	{	
		m_pVideoWnd = NULL;
	//	m_pNameWnd = NULL;

	//	m_pSignalWnd = NULL;
	//	m_pMicWnd = NULL;
	}

	CVideoUI::~CVideoUI(void)
	{
	}

	LPCTSTR CVideoUI::GetClass() const
	{
		return _T("VideoUI");
	}

	LPVOID CVideoUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, _T("VideoUI")) == 0 ) 
			return static_cast<CVideoUI*>(this);

		return CControlUI::GetInterface(pstrName);
	}

	void CVideoUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
	}

	void CVideoUI::SetInternVisible(bool bVisible)
	{
		CControlUI::SetInternVisible(bVisible);
	}

	void CVideoUI::Init()
	{
		__super::Init();
		if (IsVisible() == true)
		{
			CreateOwnWindow();
		}
	}

	void CVideoUI::DoInit()
	{
		__super::DoInit();
	}

	void CVideoUI::SetPos( RECT rc, bool bNeedInvalidate /*= true*/ )
	{
		__super::SetPos(rc, bNeedInvalidate);
		WindowPosChanged();
	}

	void CVideoUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		__super::SetAttribute(pstrName, pstrValue);
	}

	void CVideoUI::CreateOwnWindow()
	{
		if (NULL == m_pVideoWnd)
		{
			m_pVideoWnd = new CVideoWnd;
			m_pVideoWnd->Create(m_pManager->GetPaintWindow(), L"", UI_WNDSTYLE_CHILD, 0L);
			//m_pVideoWnd->SetBkColor(this->GetBkColor());
			if (GetUserData().Compare(L"teacher") == 0)
			{
				m_pVideoWnd->SetImageHorheiandwidth(125, 105);
			}
			else if (GetUserData().Compare(L"studentme") == 0)
			{
				m_pVideoWnd->SetImageHorheiandwidth(95, 79);
			}
			else if (GetUserData().Compare(L"studentone") == 0)
			{
				m_pVideoWnd->SetImageHorheiandwidth(98, 80);
			}
			else if (GetUserData().Compare(L"studenttwo") == 0)
			{
				m_pVideoWnd->SetImageHorheiandwidth(97, 79);
			}
			else if (GetUserData().Compare(L"studenttree") == 0)
			{
				m_pVideoWnd->SetImageHorheiandwidth(94, 80);
			}

			m_pVideoWnd->SetDefaultCtrolBkImage(this->GetBkImage());
		}

	}

	CVideoWnd* CVideoUI::GetOwnWindow()
	{
		return m_pVideoWnd;
	}

	void CVideoUI::WindowPosChanged()
	{
		CRect rcwindow = GetPos();
		if (rcwindow.IsRectEmpty())
		{
			return;
		}

		if (m_pVideoWnd)
		{
			::SetWindowPos(m_pVideoWnd->GetHWND(), HWND_TOP, rcwindow.left, rcwindow.top, rcwindow.Width(), rcwindow.Height(), SWP_NOACTIVATE);
		}
		
	}

}//namespace

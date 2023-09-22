/************************************************************************/
/* Video UI Control								                        */						                        */
/************************************************************************/
#pragma once
#include "VideoWnd.h"
namespace DuiLib
{
	class CVideoUI : public CHorizontalLayoutUI
	{
	public:
		CVideoUI();
		virtual ~CVideoUI(void);

		void SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue );

	public://DUILIB
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
		virtual void SetVisible(bool bVisible);
		virtual void SetInternVisible(bool bVisible = true);

		virtual void Init();
		virtual void DoInit();

		void CreateOwnWindow();
		
		CVideoWnd* GetOwnWindow();

		void WindowPosChanged();

	private:

		CVideoWnd* m_pVideoWnd;
	
	};
};


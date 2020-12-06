#pragma once

class CVideoWnd : public WindowImplBase
{
public:
	CVideoWnd();
	virtual ~CVideoWnd();
	
	void SetKid(BOOL bKid);
	void Set1V1(BOOL b1v1);
	void SetBkImage(LPCTSTR lpszBkImage);
	void SetBkColor(DWORD dwBkColor);
	void SetBkText(LPCTSTR lpszText);
	void SetBkIcon(LPCTSTR lpszBkIcon);
	void SetVideoMode(BOOL bVideo = TRUE);
	void SetDefaultCtrolBkImage(LPCTSTR lpszBkImage);
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);

	void RefreshWnd();
	BOOL IsVideoMode();

	void SetImageHorheiandwidth(int iheight, int iwidth );
	
	virtual void InitWindow();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	DuiLib::CDuiString GetSkinFolder();
	DuiLib::CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;

	static UINT GetStateChangedMessage(){return s_uMsgStateChanged;}

public:
	void  SetParentHwnd(HWND hParentWnd,UINT nNotifyEnterMsgId,UINT nNotifyLeaveMsgId, UINT nNotifyLButtonDownMsgId = 0);
	void UserEnter();
	void UserLeave();

protected:
	static UINT s_uMsgStateChanged;

	BOOL   m_bMouseTracking;
	HWND   m_hParentWnd;
	UINT   m_nNotifyEnterMsgId;
	UINT   m_nNotifyLeaveMsgId;
	UINT   m_nNotifyLButtonDownMsgId;
	BOOL   m_bkid;

	BOOL m_bVideo;
	BOOL m_b1v1;

	HWND m_hNotifyWnd;	
	BOOL m_bUserEntry;

	CVerticalLayoutUI* m_pRootVL;
	CHorizontalLayoutUI* m_pImgHL;
	CLabelUI* m_pTitleImg;
	CLabelUI* m_pTileLab;


	CLabelUI* m_pUILabelDefaultBkImage;
	CHorizontalLayoutUI* m_pHorUI;

};

#pragma once
#include "UICef3Web.h"
#include "RoomWindow.h"
class CCircleImageUI;
class CImageAnimateUI;
class CUIListItemCust;

class CMainframe : 
    public CCef3CallBack
	, public CCefContainerWnd

{
public:
	CMainframe(void);
	virtual ~CMainframe(void);

	//INotifyUI
public:
	virtual void Notify(TNotifyUI& msg);
	//CWindowWndc
protected:
	virtual LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const { return CS_DBLCLKS; };

	DuiLib::CDuiString GetSkinFolder();
	DuiLib::CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;

public:
	virtual void InitWindow();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
    virtual CControlUI* CreateControl(LPCTSTR pstrClass)override;

public:
	virtual void OnBrowserCreated(void* ThisUI);                                    // 创建完成，可以navigate		
	virtual void OnLoadEnd(void* ThisUI);                                           // Navigate完成 js上下文就绪，可以执行js代码
	virtual void OnLoadError(int nError, void* ThisUI);                             // Navigate错误      
	virtual void OnJSCallCPlus(LPCWSTR lpwsId, LPCWSTR lpwsContent, void* ThisUI);  // JS回调接口 
    virtual void OnBrowserClosed();
    virtual void OnRenderProcessTerminated(LPCWSTR lpwsUrl, int nStatus);
	virtual void OnTitleChange(LPCWSTR lpTitle, void* ThisUI);
	virtual void OnAddressChange(LPCWSTR lpwsaddress, void* ThisUI);
	virtual void OnCookie(LPCWSTR cookie, void* ThisUI);
	virtual void OnLoadingStateChange(void* ThisUI, bool bisLoading, bool isgoback, bool isgoforure);

public:
    void CalltestJS();
	
    const bool Quit(const bool bLogout = false, const bool bForce = false);

	void ShowWebLoading();
	void OnWebLoadSucc();
	void OnWebLoadErr();
	void OpenWebPage();

	void ShowTip(LPCWSTR lpText);

	void CallCusJS( LPCWSTR lpjsFile);

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRefreshBtn;

    CHorizontalLayoutUI* m_pLogoVL;
	CLabelUI* m_pNickName;
	CRichEditUI	*m_pEditNickName;
	CButtonUI	*m_pBtnModifyNickName;
	CControlUI	*m_pCtrlGap;
	CCircleImageUI* m_phandImge;

	CButtonUI* m_pBtnDevCheck;
	CButtonUI* m_pBtnExitLogin;
    CLabelUI*  m_pVersionLab;
	CButtonUI* m_pBtnMaterialMgr;
	CImageAnimateUI* m_pBtnMateriaAnimate;

	CHorizontalLayoutUI* m_pWebLoading;
	CHorizontalLayoutUI* m_pWebLoadFail;
	CHorizontalLayoutUI* m_pWebpage;
    CGifAnimUI * m_pGifLoading;
    
	CCef3WebUI* m_pCourseWeb;

	CRoomWindow* proom;

};
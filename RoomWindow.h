#pragma once
#include "UICef3Web.h"
#include "UIVideo.h"
class CRoomWindow
	: public WindowImplBase
	, public CCef3CallBack
{
public:
	CRoomWindow();
	virtual ~CRoomWindow(void);

	//INotifyUI
public:
	virtual void Notify(TNotifyUI& msg);
	//CWindowWndc
protected:
	virtual LPCTSTR GetWindowClassName() const { return L"room"; };
	UINT GetClassStyle() const { return CS_DBLCLKS; };

	DuiLib::CDuiString GetSkinFolder();
	DuiLib::CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;

public:
	virtual void InitWindow();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	//	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;
	//	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass)override;

public:
	virtual void OnBrowserCreated(void* ThisUI);                                    // 创建完成，可以navigate		
	virtual void OnLoadEnd(void* ThisUI);                                           // Navigate完成 js上下文就绪，可以执行js代码
	virtual void OnLoadError(int nError, void* ThisUI);                             // Navigate错误      
	virtual void OnJSCallCPlus(LPCWSTR lpwsId, LPCWSTR lpwsContent, void* ThisUI);  // JS回调接口 
	virtual void OnBrowserClosed() {};
	virtual void OnRenderProcessTerminated(LPCWSTR lpwsUrl, int nStatus) {};
	virtual void OnTitleChange(LPCWSTR lpTitle, void* ThisUI) {};
	virtual void OnAddressChange(LPCWSTR lpwsaddress, void* ThisUI) {};
	virtual void OnCookie(LPCWSTR cookie, void* ThisUI) {};
	virtual void OnLoadingStateChange(void* ThisUI, bool bisLoading, bool isgoback, bool isgoforure) {};

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRefreshBtn;

	CCef3WebUI* m_pWBUI;
	CVideoUI* m_meUI;
	CVideoUI* m_youUI;
};
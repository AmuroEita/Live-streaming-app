#pragma once
#include "UICef3Web.h"

class CHelpWebWindow : public CCefContainerWnd
	, public CCef3CallBack
{
public:
	CHelpWebWindow(IOwnerWndManager* pOwnerWndManager);
	virtual ~CHelpWebWindow(void);

	//INotifyUI
public:
	virtual void Notify(TNotifyUI& msg);

	//CCefContainerWnd
	virtual void DoClose()override;
	//CWindowWndc
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	UINT GetClassStyle() const { return CS_DBLCLKS; };

	DuiLib::CDuiString GetSkinFolder();
	DuiLib::CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	//     LPCTSTR GetResourceID() const
	//     {
	//         return MAKEINTRESOURCE(IDR_SKIN);
	//     }
	virtual CDuiString GetZIPFileName() const;

	CControlUI* CreateControl(LPCTSTR pstrClass);
public:
	virtual void InitWindow();
	void SetURL(LPCWSTR url);
	void OpenPage();

public:
	virtual void OnBrowserCreated(void* ThisUI);                                    // 创建完成，可以navigate		
	virtual void OnLoadEnd(void* ThisUI);                                           // Navigate完成 js上下文就绪，可以执行js代码
	virtual void OnLoadError(int nError, void* ThisUI);                             // Navigate错误      
	virtual void OnJSCallCPlus(LPCWSTR lpwsId, LPCWSTR lpwsContent, void* ThisUI);  // JS回调接口 
	virtual void OnBrowserClosed();
	virtual void OnRenderProcessTerminated(LPCWSTR lpwsUrl, int nStatus);
	virtual void OnTitleChange(LPCWSTR lpTitle, void* ThisUI) {};
private:
	CButtonUI* m_pCloseBtn;

	CVerticalLayoutUI* m_RootVLayoutUI;

	CHorizontalLayoutUI* m_pHorLoading;
	CHorizontalLayoutUI* m_pHorHelpWeb;
	CHorizontalLayoutUI* m_pHorHelpFailPage;

	CCef3WebUI* m_pHelpWeb;
private:
    IOwnerWndManager* m_pOwnerWndManager;

	CString m_strurl;
};
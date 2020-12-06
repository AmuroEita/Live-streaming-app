/************************************************************************/
/* Cef3 UI Control								                        */
/* author : LixueGang							                        */
/************************************************************************/
#pragma once
#include "CefWndWrapManager.h"
namespace DuiLib
{
	class CCef3CallBack
	{
	public:
		virtual void OnBrowserCreated(void* ThisUI)PURE;                                    // 创建完成，可以navigate		
		virtual void OnLoadEnd(void* ThisUI)PURE;                                           // Navigate完成 js上下文就绪，可以执行js代码
		virtual void OnLoadError(int nError, void* ThisUI)PURE;                             // Navigate错误      
		virtual void OnJSCallCPlus(LPCWSTR lpwsId, LPCWSTR lpwsContent, void* ThisUI)PURE;  // JS回调接口 
		virtual void OnBrowserClosed()PURE;
		virtual void OnRenderProcessTerminated(LPCWSTR lpwsUrl, int nStatus) PURE;
		virtual void OnTitleChange(LPCWSTR lpTitle, void* ThisUI)PURE;
		virtual void OnAddressChange(LPCWSTR lpwsaddress, void* ThisUI)PURE;
		virtual void OnCookie(LPCWSTR cookie, void* ThisUI)PURE;	
		virtual void OnLoadingStateChange(void* ThisUI, bool bisLoading, bool isgoback, bool isgoforure)PURE;
	};

	class CCef3WebUI
		: public CCefUIBase
	{
	public:
		CCef3WebUI(void);
		virtual ~CCef3WebUI(void);

	public://DUILIB
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
		virtual void SetVisible(bool bVisible);
		virtual void SetInternVisible(bool bVisible = true);

	public:	//IBrowserEvent
		STDMETHOD(OnBrowserCreated)(LPCWSTR lpwsFlag);
		STDMETHOD(OnLoadEnd)(LPCWSTR lpwsUrl, BOOL bMain);
		STDMETHOD(OnLoadError)(LPCWSTR lpwsUrl, BOOL bMain, int nError);
		STDMETHOD(OnJSCallCPlus)(LPCWSTR lpwsUrl, LPCWSTR lpwsId, LPCWSTR lpwsContent);
		STDMETHOD(OnLoadHtmlSize)(LPCWSTR lpwsUrl, LONGLONG llSize);
		STDMETHOD(OnSnapShot)(IRingBuffer* spBuffer, int nWidth, int nHeight);
        STDMETHOD(OnBrowserClosed)(int);
		STDMETHOD(OnRenderProcessTerminated)(LPCWSTR lpwsUrl, int nStatus, int nReserved);
		STDMETHOD(OnTitleChange)(LPCWSTR lpwsTitle);
		STDMETHOD(OnAddressChange)(LPCWSTR lpwsaddress);
		STDMETHOD(OnGetCookie)(LPCWSTR lpwsTitle);
		STDMETHOD(OnLoadingStateChange)(bool bisLoading, bool isgoforure, bool isgoback);

	public:
		void SetCef3CallBack(CCef3CallBack* pCallBack);
//        virtual bool CreateBrowser(LPCTSTR szName=NULL, LPRECT rcBrowser = NULL)override;
//        virtual void CloseBrowser() override;
        const HWND GetBrowserHWND() const;
        //void DestroyBrowser();

		void OpenURL(LPCWSTR lpUrl, BOOL bIgnore404 = TRUE);
		void Reload();
		
		void GoBack();
		void GoForward();

		HRESULT CallJSCommonFunc(LPCWSTR lpwsJSFuncName, LPCWSTR lpwsJSFuncParamJson);
		HRESULT CallJS(LPCWSTR lpwsJS);
		HRESULT CallJS2(ICefStringArray* lpJS);
		HRESULT SelectAll();
		HRESULT CopySelect();
		HRESULT InvalidateBrowserWndRect(LPRECT lprc, BOOL bRedraw);
		HRESULT Stop();
		HRESULT LogError(LPCWSTR lpwsId, LPCWSTR lpwsContent, LPCWSTR lpwsError);

	public:
		CCef3CallBack* m_pCef3Callback;

	public:
		static HRESULT CreateCEFStrignArray(ICefStringArray** lppArr);

	};
}

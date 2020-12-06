#include "stdafx.h"
#include "UICef3Web.h"
namespace DuiLib
{
	CCef3WebUI::CCef3WebUI(void) 
        : m_pCef3Callback(NULL)
	{
		
	}

	CCef3WebUI::~CCef3WebUI(void)
	{
		LOGPRINTTIMESTAMP(L"CCef3WebUI::~CCef3WebUI");
	}

	LPCTSTR CCef3WebUI::GetClass() const
	{
		return _T("UICCef3Web");
	}

	LPVOID CCef3WebUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, _T("Cef3Web")) == 0 ) 
			return static_cast<CCef3WebUI*>(this);

		return CControlUI::GetInterface(pstrName);
	}

	void CCef3WebUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if(m_spCefWebBrowser)
			m_spCefWebBrowser->SetShow(bVisible?TRUE:FALSE);
	}

	void CCef3WebUI::SetInternVisible(bool bVisible)
	{
		CControlUI::SetInternVisible(bVisible);
		if(m_spCefWebBrowser)
			m_spCefWebBrowser->SetShow(bVisible?TRUE:FALSE);

		if (bVisible)
			InvalidateBrowserWndRect(&m_rcItem, true);
	}

	void CCef3WebUI::SetCef3CallBack( CCef3CallBack* pCallBack )
	{
		if (NULL == m_pCef3Callback)
		{
			m_pCef3Callback = pCallBack;
		}
	}

	void CCef3WebUI::OpenURL( LPCWSTR lpUrl, BOOL bIgnore404/* = TRUE*/)
	{
		if(m_spCefWebBrowser)
			m_spCefWebBrowser->LoadURL(lpUrl, bIgnore404);
	}

	void CCef3WebUI::Reload()
	{
		if(m_spCefWebBrowser)
			m_spCefWebBrowser->Reload(FALSE);
	}

	void CCef3WebUI::GoBack()
	{
		if (m_spCefWebBrowser)
		{
			m_spCefWebBrowser->Goback();
		}
	}

	void CCef3WebUI::GoForward()
	{
		if (m_spCefWebBrowser)
		{
			m_spCefWebBrowser->GoForward();
		}
	}

	//     bool CCef3WebUI::CreateBrowser(LPCTSTR szName, LPRECT rcBrowser)
//     {
//         if (__super::CreateBrowser(szName, rcBrowser))
//         {
//             m_spCefWebBrowser->AdvisEvent(this);
//             return true;
//         }
//         return false;
//     }
// 
//     void CCef3WebUI::CloseBrowser()
//     {
//         if (m_spCefWebBrowser)
//         {
//             CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
//             spWB2->Close();
//         }
//     }

    const HWND CCef3WebUI::GetBrowserHWND() const
    {
        return m_hBrowerWnd;
    }

//     void CCef3WebUI::DestroyBrowser()
//     {
//         if (m_hBrowerWnd)
//         {
//             if (::IsWindow(m_hBrowerWnd))
//                 DestroyWindow(m_hBrowerWnd);
//             m_hBrowerWnd = NULL;
//         }
// 
//         if (m_spCefWebBrowser)
//             m_spCefWebBrowser->AdvisEvent(NULL);
//         m_spCefWebBrowser.Release();
// 
//         m_pCef3Callback = NULL;
//     }

    void CCef3WebUI::SetPos(RECT rc, bool bNeedInvalidate /*= true*/)
	{
		__super::SetPos(rc, bNeedInvalidate);

		if(m_spCefWebBrowser)
		{
			m_spCefWebBrowser->AdjustRect(&rc);
		}
	}

	HRESULT CCef3WebUI::CallJSCommonFunc(LPCWSTR lpwsJSFuncName, LPCWSTR lpwsJSFuncParamJson)
	{
		if (!lpwsJSFuncName || !lpwsJSFuncParamJson)
			return E_FAIL;

		CString strJSFuncName =lpwsJSFuncName;
		CString strJSFuncParamJson = lpwsJSFuncParamJson;

		{
			strJSFuncName = MakeSafeJSString(lpwsJSFuncName);
			strJSFuncParamJson = MakeSafeJSString(lpwsJSFuncParamJson);
		}

		CString str;
		str.Format(L"comm_type_get('%s', '%s')", strJSFuncName, strJSFuncParamJson);
		LOGPRINTTIMESTAMP(L"CCef3WebWindow::CallJSCommonFunc %s", (LPCTSTR)str);
		return CallJS(str);
	}
	
	HRESULT CCef3WebUI::CallJS(LPCWSTR lpwsJS)
	{
		if(NULL == lpwsJS)
			return E_INVALIDARG;

		if(m_spCefWebBrowser)
			return m_spCefWebBrowser->CallJS(lpwsJS);
		return E_FAIL;
	}

	HRESULT CCef3WebUI::CallJS2(ICefStringArray* lpJS)
	{
		LOGPRINTTIMESTAMP(L"CCef3WebWindow::[%#X]CallJS2()", (DWORD_PTR)this);
		CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
		if(!spWB2)
			return E_FAIL;

		return spWB2->CallJS2(lpJS);
	}

	HRESULT CCef3WebUI::SelectAll()
	{
		CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
		if(!spWB2)
			return E_FAIL;

		return spWB2->SelectAll();
	}

	HRESULT CCef3WebUI::CopySelect()
	{
		CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
		if(!spWB2)
			return E_FAIL;

		return spWB2->CopySelect();
	}

	HRESULT CCef3WebUI::Stop()
	{
		if (!m_spCefWebBrowser)
			return E_FAIL;
		return m_spCefWebBrowser->Stop();
	}

	HRESULT CCef3WebUI::LogError(LPCWSTR lpwsId, LPCWSTR lpwsContent, LPCWSTR lpwsError)
	{
		if (!m_spCefWebBrowser)
			return E_FAIL;
		return m_spCefWebBrowser->LogError(lpwsId, lpwsContent, lpwsError);
	}

	STDMETHODIMP CCef3WebUI::OnBrowserCreated(LPCWSTR lpwsFlag)
	{
		LOGPRINTTIMESTAMP(L"CCef3WebWindow::[%#X]OnBrowserCreated(%s)", (DWORD_PTR)this, lpwsFlag?lpwsFlag:L"NOFLAG");	
		CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
		if(spWB2)
			m_hBrowerWnd = spWB2->GetBrowserWindow();

		if (m_pCef3Callback )
		{
			m_pCef3Callback->OnBrowserCreated(this);
		}
		m_nTimeCreate = ::GetTickCount();
        NotifyCreated();
		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnLoadError(LPCWSTR lpwsUrl, BOOL bMain, int nError)
	{
		if (!bMain)
			return S_OK;

		LOGPRINTTIMESTAMP(L"CCef3WebWindow::[%#X]OnLoadError(%d, %s)", (DWORD_PTR)this, nError, lpwsUrl?lpwsUrl:L"NOURL");
		if (m_pCef3Callback )
		{
			m_pCef3Callback->OnLoadError(nError, this);
		}
		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnJSCallCPlus(LPCWSTR lpwsUrl, LPCWSTR lpwsId, LPCWSTR lpwsContent)
	{
        if (lpwsUrl == NULL || lpwsId == NULL || lpwsContent == NULL)
        {
            return S_FALSE;
        }
		LOGPRINTTIMESTAMP(L"CCef3WebUI[%s]::OnJSCallCPlus(%s, %s)[%#X][%s]", GetClass(), lpwsId, lpwsContent, this, lpwsUrl);
		if (m_pCef3Callback )
		{
			m_pCef3Callback->OnJSCallCPlus(lpwsId, lpwsContent, this);
		}
		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnLoadHtmlSize(LPCWSTR lpwsUrl, LONGLONG llSize)
	{
		m_nLoadSize = llSize;
		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnSnapShot(IRingBuffer* spBuffer, int nWidth, int nHeight)
	{
		if (spBuffer)
		{
			CComPtr<IRingBuffer> spRingBuffer;
			spRingBuffer.Attach(spBuffer);
		}
		return S_OK;
	}

    STDMETHODIMP CCef3WebUI::OnBrowserClosed(int)
    {
        if (m_pCef3Callback)
        {
            m_pCef3Callback->OnBrowserClosed();
        }
        NotifyClosed();
        return S_OK;
    }

    STDMETHODIMP CCef3WebUI::OnRenderProcessTerminated(LPCWSTR lpwsUrl, int nStatus, int nReserved)
    {
        if (m_pCef3Callback)
        {
            m_pCef3Callback->OnRenderProcessTerminated(lpwsUrl, nStatus);
        }
        return S_OK;
    }


	STDMETHODIMP CCef3WebUI::OnTitleChange(LPCWSTR lpwsTitle)
	{
		if (m_pCef3Callback)
		{
			m_pCef3Callback->OnTitleChange(lpwsTitle, this );
		}
		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnAddressChange(LPCWSTR lpwsaddress)
	{
		if (m_pCef3Callback)
		{
			m_pCef3Callback->OnAddressChange(lpwsaddress, this);
		}
		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnGetCookie(LPCWSTR lpcookie)
	{
		if (m_pCef3Callback)
		{
			m_pCef3Callback->OnCookie(lpcookie, this);
		}

		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnLoadingStateChange(bool bisLoading, bool isgoforure, bool isgoback)
	{
		if (m_pCef3Callback)
		{
			m_pCef3Callback->OnLoadingStateChange(this, bisLoading, isgoback, isgoforure);
		}

		return S_OK;
	}

	STDMETHODIMP CCef3WebUI::OnLoadEnd(LPCWSTR lpwsUrl, BOOL bMain)
	{
		if (!bMain)
			return S_OK;

		LOGPRINTTIMESTAMP(L"CCef3WebWindow::[%#X]OnLoadEnd(%s)", (DWORD_PTR)this, lpwsUrl?lpwsUrl:L"NOURL");

		if (m_pCef3Callback )
		{
			m_pCef3Callback->OnLoadEnd(this);
		}
		return S_OK;
	}

 	HRESULT CCef3WebUI::CreateCEFStrignArray(ICefStringArray** lppArr)
 	{
 		//CComPtr<IServiceModule> spModule;
 		//HRESULT hr = g_ACTalkApp.GetService(SID_ServiceModule, &spModule);
 		//if(!spModule)
 		//	return hr;
 
 		//CComPtr<ICefStringArray> spCEFStrArray;
 		//hr = spModule->GetObject(MODULE_ID_ACCEF, CLSID_CefStringArray, __uuidof(ICefStringArray), (LPVOID*)&spCEFStrArray);
 		//if(!spCEFStrArray)
 		//	return hr;
 
 		//return spCEFStrArray.QueryInterface(lppArr);
        return S_OK;
 	}

	HRESULT CCef3WebUI::InvalidateBrowserWndRect(LPRECT lprc, BOOL bRedraw)
	{
		if(m_hBrowerWnd && ::IsWindow(m_hBrowerWnd))
		{
			::InvalidateRect(m_hBrowerWnd, lprc, bRedraw);
			::UpdateWindow(m_hBrowerWnd);
		}
		return S_OK;
	}
}//namespace

#include "stdafx.h"
#include "CefWndWrapManager.h"

static const int WM_CEFMANAGER_ADD = WM_USER + 8001;
static const int WM_CEFMANAGER_DELETE = WM_USER + 8002;

CString CCefUIBase::MakeSafeJSString(LPCWSTR szStr)
{
    if (!szStr)
        return L"";

    CString arrSpecJSChar[9] =
    {
        L"\\",
        L"'",
        L"\"",
        L"&",
        L"\n",
        L"\r",
        L"\t",
        L"\b",
        L"\f",
    };

    CString arrSpecNewJSChar[9] =
    {
        L"\\\\",
        L"\\'",
        L"\\\"",
        L"\\&",
        L"\\n",
        L"\\r",
        L"\\t",
        L"\\b",
        L"\\f",
    };

    CString strJS = szStr;
    for (int i = 0; i < 9; i++)
    {
        strJS.Replace(arrSpecJSChar[i], arrSpecNewJSChar[i]);
    }
    return strJS;
}

CCefUIBase::CCefUIBase()
    : m_pContainer(NULL)
    , m_hBrowerWnd(NULL)
    , m_nLoadSize(0)
    , m_nTimeCreate(::GetTickCount())
    , m_bCalledClose(false)
{

}

CCefUIBase::~CCefUIBase()
{
	HWND hw = GetBrowserHWND();
	DestroyWindow(hw);
}

void CCefUIBase::AttachContainer(CCefContainerWnd * pContainer)
{
    m_pContainer = pContainer;
    if (m_pContainer)
    {
        m_pContainer->AddCef(this);
    }
}

bool CCefUIBase::CreateBrowser(LPCTSTR szName /*= NULL*/, LPRECT rcBrowser /*= NULL*/, LPCWSTR lpcookiepath/*= NULL*/, LPCWSTR lpcookieServer/* = NULL*/, LPCWSTR lpdomain/* = NULL*/)
{
    if (!m_spCefWebBrowser)
    {
        HWND hWndParen = NULL;
        CPaintManagerUI* pPM = GetManager();
        if (pPM)
            hWndParen = pPM->GetPaintWindow();

        CRect rcWindow;
        if (rcBrowser)
            rcWindow = rcBrowser;
        else
            rcWindow = GetPos();
        CComPtr<ICefManager> spCefMgr;
        spCefMgr = g_SACMoudleApp.GetCefMgrInstance();
        if (hWndParen && spCefMgr)
        {
            CString strBrowserName = BROWSER_OTHER;
            if (szName)
                strBrowserName = szName;

            if (SUCCEEDED(spCefMgr->CreateBrowser(hWndParen, &rcWindow, strBrowserName, &m_spCefWebBrowser, lpcookiepath, lpcookieServer, lpdomain)))
            {
                m_spCefWebBrowser->AdvisEvent(this);
                return true;
            }
        }
        ATLASSERT(FALSE);
        return true;
    }
    return false;
}

bool CCefUIBase::CloseBrowserOther() 
{
	m_bCalledClose = true;
	if (m_spCefWebBrowser)
	{
		CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
		HWND m_hOtherHWnd = spWB2->GetBrowserWindow();
		if (IsWindow(m_hOtherHWnd))
		{
			//::PostMessage(m_hOtherHWnd, WM_NCDESTROY, 0, 0);
		}

		m_spCefWebBrowser->AdvisEvent(NULL);
	}
	return false;
}

void CCefUIBase::GetCookie(LPCTSTR lpurl, LPCTSTR strcookiepath)
{
	if (m_spCefWebBrowser)
	{
		CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
		spWB2->GetUrlCookie(lpurl, strcookiepath);
	}
}

bool CCefUIBase::CloseBrowser()
{
	m_bCalledClose = true;
	if (m_spCefWebBrowser)
	{
		CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
		if (spWB2->Close() == S_OK)
		{
			//这时候已经不需要底层再给我通知
			//m_spCefWebBrowser->AdvisEvent(NULL);
			return true;
		}
	}
	return false;
}

// bool CCefUIBase::CloseBrowser()
// {
//     m_bCalledClose = true;
//     if (m_spCefWebBrowser)
//     {
//         CComQIPtr<ICefBrowser2> spWB2 = m_spCefWebBrowser;
//         if (spWB2->Close() == S_OK)
//         {
//             return true;
//         }
//         //这时候已经不需要底层再给我通知
//         m_spCefWebBrowser->AdvisEvent(NULL);
//     }
//     return false;
// }

const HWND CCefUIBase::GetBrowserHWND() const
{
    return m_hBrowerWnd;
}

void CCefUIBase::NotifyCreated()
{
    if (m_pContainer)
    {
        m_pContainer->OnCefCreated(this);
    }
}

void CCefUIBase::NotifyClosed()
{
    if (m_pContainer)
    {
        m_pContainer->OnCefClosed(this);
    }
}

CCefContainerWnd::CCefContainerWnd(bool bAutoDelete/* = true*/)
    : m_vCefList(NULL)
    , m_funClosed(NULL)
    , m_eCloseStep(sc_nomal)
    , m_pFather(NULL)
    , m_bFatherReal(false)
    , m_bAutoDelete(bAutoDelete)
{
    CCefWndWrapManager::AddItem((CCefContainerWnd*)this);
}

CCefContainerWnd::~CCefContainerWnd()
{

}

void CCefContainerWnd::OnCefCreated(CCefUIBase * pCef)
{
    
}

void CCefContainerWnd::OnCefClosed(CCefUIBase * pCef)
{
    LOGPRINTTIMESTAMP(L"[%x]CCefContainerWnd::OnCefClosed!", this);
    RemoveCef(pCef);
    _TryFinalClose();
	
}

bool CCefContainerWnd::IsClosing()
{
    return m_eCloseStep > sc_nomal;
}

void CCefContainerWnd::SafeClose(std::function<void()> funClosed)
{
    if (m_eCloseStep > sc_nomal) return;
    m_eCloseStep = sc_doClose;

    m_funClosed = funClosed;
    if (m_vChilds.size()>0)
    {
        LOGPRINTTIMESTAMP(L"[%x]CCefContainerWnd::SafeClose have child!", this);
        std::vector<CCefContainerWnd*> vTp = m_vChilds;
        for (auto p: vTp)
        {
            if (!p->IsClosing())
            {
                p->SafeClose(NULL);
            }
        }
    }
    else
    {
        LOGPRINTTIMESTAMP(L"[%x]CCefContainerWnd::SafeClose no child!", this);
        if (::IsWindow(m_hWnd))
            Close(1);
        else
            _TryFinalClose();
    }
}

void CCefContainerWnd::AddCef(CCefUIBase * pCef)
{
    if (pCef)
    {
        RemoveCef(pCef);
        m_vCefList.push_back(pCef);
    }
}

bool CCefContainerWnd::RemoveCef(CCefUIBase * pCef, bool bEnumChild/* = false*/)
{
    auto it = std::find_if(m_vCefList.begin(), m_vCefList.end(), [pCef](const CCefUIBase* pItem) {
        return pCef == pItem;
    });

    if (it != m_vCefList.end())
    {
        m_vCefList.erase(it);
        return true;
    }
    else if (bEnumChild)
    {
        for (auto p : m_vChilds)
        {
            if (p->_IsRealFather(this) && p->RemoveCef(pCef, bEnumChild))
            {
                return true;
            }
        }
    }

    return false;
}

void CCefContainerWnd::AddChildCefContainer(CCefContainerWnd* pChild, bool bRealChild/* = false*/)
{
    if (pChild)
    {
        pChild->_SetFather(this, bRealChild);
        //先移除，再增加，以防重复
        RemoveChildCefContainer(pChild);
        m_vChilds.push_back(pChild);
    }
}

void CCefContainerWnd::RemoveChildCefContainer(CCefContainerWnd* pChild)
{
    auto it = std::find_if(m_vChilds.begin(), m_vChilds.end(), [pChild](const CCefContainerWnd* pItem) {
        return DWORD(pChild) == DWORD(pItem); 
    });

    if (it != m_vChilds.end())
    {
        m_vChilds.erase(it);
    }
}

void CCefContainerWnd::WaitLastMessageLoop(int iCount, int iDelay)
{
    for (int i = 0; i < iCount; i++)
    {
        WaitLastMessage();
        ::Sleep(iDelay);
    }
}

LRESULT CCefContainerWnd::OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    //如果未曾主动调用关闭，直接忽略这个消息，因为可能是子窗口的关闭触发的
    //如果只是子窗口关闭，cef的销毁消息也会乱，所以对此情况暂不多做处理，上层得控制
    if (m_eCloseStep == sc_nomal)
    {
        bHandled = TRUE;
	//	::DestroyWindow(m_hWnd);
    }
    else
    {
        if (m_eCloseStep != sc_closing)
        {
            m_eCloseStep = sc_closing;
            DoClose();
        }

        //不判断wParam了，枚举没有关闭的cef，有则截获此关闭消息
        bHandled = !!_EnumAndCloseCef(true);
    }

    LOGPRINTTIMESTAMP(L"[%x]CCefContainerWnd::OnClose(1) handed(%d) step(%d)", this, bHandled, m_eCloseStep);

    return 0;
}

LRESULT CCefContainerWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    __super::OnDestroy(uMsg, wParam, lParam, bHandled);
    bHandled = TRUE;
    return 0;
}

void CCefContainerWnd::OnFinalMessage(HWND hWnd)
{
    __super::OnFinalMessage(hWnd);
    //如果cef关闭异常，这里直接触发删除
    _TryFinalClose();
}

DuiLib::CControlUI* CCefContainerWnd::CreateControl(LPCTSTR pstrClass)
{
    if (_tcsicmp(pstrClass, _T("Cef3Web")) == 0)
    {
        CCefUIBase * pCefUI = new CCef3WebUI;
        pCefUI->AttachContainer(this);
        return pCefUI;
    }
    
    return __super::CreateControl(pstrClass);
}

void CCefContainerWnd::OnChildClosed(CCefContainerWnd* pChild, bool bReal)
{
    LOGPRINTTIMESTAMP(L"[%x]CCefContainerWnd::OnChildClosed(%x)", this, pChild);
    RemoveChildCefContainer(pChild);
    //所有子对象都关闭后，才能关闭自己
    if (m_vChilds.size() <= 0 && m_eCloseStep == sc_doClose)
    {
        Close(1);
    }
}

void CCefContainerWnd::_SetFather(CCefContainerWnd* pFather, bool bReal/* = false*/)
{
    m_pFather = pFather;
    m_bFatherReal = bReal;
}

bool CCefContainerWnd::_EnumAndCloseCef(bool bEnumChild /*= false*/)
{
    //先去遍历真子窗口中的cef,进行关闭，因为真子窗口中的cef关闭会直接触发真顶级父窗的关闭消息
    if (bEnumChild)
    {
        for (auto p : m_vChilds)
        {
            if (p->_IsRealFather(this) && p->_EnumAndCloseCef(bEnumChild))
            {
                return true;
            }
        }
    }

    CCefUIBase * pCef = _GetUnCloseCef();
    if (pCef)
    {
        if (pCef->CloseBrowser())
        {
            return true;
        }
        else
        {
            RemoveCef(pCef);
        }
    }

    return false;
}

CCefUIBase * CCefContainerWnd::_EnumUnCloseCef(bool bEnumChild/* = false*/)
{
    CCefUIBase * pCef = NULL;
    //先去遍历真子窗口中的cef,进行关闭，因为真子窗口中的cef关闭会直接触发真顶级父窗的关闭消息
    if (bEnumChild)
    {
        for (auto p : m_vChilds)
        {
            if (p->_IsRealFather(this))
            {
                pCef = p->_EnumUnCloseCef(bEnumChild);
                if (pCef) return pCef;
            }
        }
    }
    
    pCef = _GetUnCloseCef();

    return pCef;
}

CCefUIBase * CCefContainerWnd::_GetUnCloseCef()
{
    for (auto p : m_vCefList)
    {
        if (!p->IsCalledClose())
            return p;
    }
    return NULL;
}

bool CCefContainerWnd::_IsRealFather(CCefContainerWnd* pFather)
{
    return m_bFatherReal && pFather && m_pFather && m_pFather == pFather;
}

void CCefContainerWnd::_TryFinalClose()
{
    if (m_vCefList.size() <= 0)
    {
        //最后的清理工作，承上启下，通知父窗口，将自己移除列表，并清空自己的子窗口
        if (m_pFather) m_pFather->OnChildClosed(this, m_bFatherReal);
        //对子窗口做清理工作
        if (m_vChilds.size()>0)
        {
            for (auto p : m_vChilds)
            {
                p->_SetFather(NULL);
            }
        }
        m_vChilds.clear();
        
        CCefWndWrapManager::RemoveItem(this, m_bAutoDelete);
    }
}

HWND CCefWndWrapManager::m_hMgrWnd = NULL;
std::vector<CCefContainerWnd*> CCefWndWrapManager::m_vWrapList;

CCefWndWrapManager::CCefWndWrapManager(void)
{

}

CCefWndWrapManager::~CCefWndWrapManager(void)
{

}

void CCefWndWrapManager::Init()
{
    if (m_hMgrWnd == NULL)
    {
        m_hMgrWnd = _CreateWnd(_MsgWndProc, NULL, L"");
    }
}

void CCefWndWrapManager::UnInit()
{
    if (m_hMgrWnd != NULL)
    {
        ::DestroyWindow(m_hMgrWnd);
        m_hMgrWnd = NULL;
    }

    if (m_vWrapList.size() > 0)
    {
        for (auto p : m_vWrapList)
        {
            if(p->m_bAutoDelete)
                delete p;
        }
    }

    std::vector<CCefContainerWnd*>().swap(m_vWrapList);   
}

void CCefWndWrapManager::AddItem(CCefContainerWnd * pItem, bool bAsyn/* = true*/)
{
    if (bAsyn)
    {
        ::PostMessage(m_hMgrWnd, WM_CEFMANAGER_ADD, NULL, LPARAM(pItem));
    }
    else
    {
        CCefWndWrapManager::_AddItem(pItem);
    }
}

void CCefWndWrapManager::RemoveItem(CCefContainerWnd * pItem, bool bAsyn/* = true*/)
{
    if (bAsyn)
    {
        ::PostMessage(m_hMgrWnd, WM_CEFMANAGER_DELETE, NULL, LPARAM(pItem));
    }
    else
    {
        CCefWndWrapManager::_RemoveItem(pItem);
    }
}

LRESULT __stdcall CCefWndWrapManager::_MsgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    case WM_NCCREATE:
        //::SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG)(LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams));
        break;
    case WM_CEFMANAGER_ADD:
    {
        /*CCefWndManager * pMgr = ((CIPCWnd*)(LONG_PTR)::GetWindowLongPtr(hWnd, GWL_USERDATA));
        if (pMgr != NULL)
        {
        }*/
        CCefWndWrapManager::_AddItem((CCefContainerWnd*)lParam);
    }
    break;
    case WM_CEFMANAGER_DELETE:
    {
        /*CCefWndManager * pMgr = ((CIPCWnd*)(LONG_PTR)::GetWindowLongPtr(hWnd, GWL_USERDATA));
        if (pMgr != NULL)
        {
        }*/
        CCefWndWrapManager::_RemoveItem((CCefContainerWnd*)lParam);
    }
    break;
    default:
        break;
    }
    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

const HWND CCefWndWrapManager::_CreateWnd(const WNDPROC pfnWndProc, void * pData, const wchar_t * pwszWndName /*= NULL*/, const wchar_t * pwszClassName /*= NULL*/)
{
    if (pfnWndProc == NULL)
        return NULL;

    if (pwszClassName == NULL || wcslen(pwszClassName) <= 0)
    {
        pwszClassName = L"CEFMANAGERWNDCLASS";
    }

    WNDCLASSEX wndclassex = { 0 };
    wndclassex.cbSize = sizeof(WNDCLASSEX);
    wndclassex.style = CS_HREDRAW | CS_VREDRAW;
    wndclassex.lpfnWndProc = pfnWndProc;
    wndclassex.cbClsExtra = 0;
    wndclassex.cbWndExtra = 0;
    wndclassex.hInstance = GetModuleHandle(NULL);
    wndclassex.hIcon = NULL; //LoadIcon(NULL, IDI_APPLICATION);
    wndclassex.hIconSm = NULL;
    wndclassex.hCursor = NULL;
    wndclassex.hbrBackground = NULL;
    wndclassex.lpszMenuName = NULL;
    wndclassex.lpszClassName = pwszClassName;

    ATOM atRet = RegisterClassEx(&wndclassex);
    if (atRet == 0L && ::GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return NULL;

    return CreateWindow(pwszClassName, pwszWndName ? pwszWndName : L"", 0, 0, 0, 0, 0, HWND_MESSAGE, 0, GetModuleHandle(NULL), pData);
}

void CCefWndWrapManager::_AddItem(CCefContainerWnd * pItem)
{
    if (!pItem) return;

    auto it = ::find_if(m_vWrapList.begin(), m_vWrapList.end(), [pItem](CCefContainerWnd * p) {
        return p == pItem;
    });

    if (it != m_vWrapList.end())
    {
        return;
    }

    m_vWrapList.push_back(pItem);
}

void CCefWndWrapManager::_RemoveItem(CCefContainerWnd * pItem)
{
    if (!pItem) return;

    auto it = ::find_if(m_vWrapList.begin(), m_vWrapList.end(), [pItem](CCefContainerWnd * p) {
        return p == pItem;
    });

    if (it != m_vWrapList.end())
    {
        if(it != m_vWrapList.end()) m_vWrapList.erase(it);

        auto funClosed = pItem->m_funClosed;
        if (pItem->m_bAutoDelete)
        {
            delete pItem;
        }

        if (funClosed)
        {
            funClosed();
        }
    }
}

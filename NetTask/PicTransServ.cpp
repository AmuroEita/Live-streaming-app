#include "stdafx.h"
#include "PicTransServ.h"

CPicTransServ::CPicTransServ() 
    : CTaskThreadBase(true, 10)
{
    Init();
}

CPicTransServ::~CPicTransServ()
{
    UnInit();
}

void CPicTransServ::Init()
{
    RECT rcWnd;
    rcWnd.left	= rcWnd.top		= 0;
    rcWnd.right	= rcWnd.bottom	= 1;
    CWindowImpl<CPicTransServ>::Create(NULL, rcWnd, NULL
        , WS_OVERLAPPEDWINDOW);
}

void CPicTransServ::UnInit()
{
    CWindowImpl<CPicTransServ>::DestroyWindow();
}

void CPicTransServ::OnTaskCompleted(CTaskItemBase* pTask)
{
    CPicTransTask* pPicTask = dynamic_cast<CPicTransTask*>(pTask);
    if (NULL == pPicTask)
    {
        CTaskThreadBase::OnTaskCompleted(pTask);
        return;
    }
    
    if (pPicTask->NeedRetry())
    {
        AddToRetryList(pPicTask);
    }
    else
    {
        CTaskThreadBase::OnTaskCompleted(pTask);
    }
}

void CPicTransServ::AddToRetryList(CPicTransTask* pTask)
{
    CMTGurad<CCriticalSect> guard(m_transTaskListLock);

    bool bSetTimer = m_transTaskList.empty();
    m_transTaskList.push_back(pTask);
    if (bSetTimer)
        SetRetryNetReqTimer();
}

bool CPicTransServ::TaskIsExist(LPCWSTR lpFilePath)
{
    CMTGurad<CCriticalSect> guard(m_transTaskListLock);

    auto it = std::find_if(m_transTaskList.begin(), m_transTaskList.end(), [lpFilePath](CPicTransTask* pTask) {
        return pTask->GetLocalPath() == lpFilePath;
    });

    if (it != m_transTaskList.end())
        return true;

    return false;
}

void CPicTransServ::RemoveTaskByPath(LPCWSTR lpFilePath)
{
    CMTGurad<CCriticalSect> guard(m_transTaskListLock);

    auto it = std::find_if(m_transTaskList.begin(), m_transTaskList.end(), [lpFilePath](CPicTransTask* pTask) {
        return pTask->GetLocalPath() == lpFilePath;
    });

    if (it != m_transTaskList.end())
    {
        m_transTaskList.erase(it);
    }
}

LRESULT CPicTransServ::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UINT uTimerID = (UINT)wParam;
    if (RETRY_PIC_NET_REQ_TIMERID == uTimerID)
    {
        OnRetryNetReqTimer();
    }
    
    return TRUE;
}

void CPicTransServ::SetRetryNetReqTimer()
{
    KillTimer(RETRY_PIC_NET_REQ_TIMERID);
    SetTimer(RETRY_PIC_NET_REQ_TIMERID, RETRY_PIC_NET_REQ_TIMEOUT, NULL);
}

void CPicTransServ::KillRetryNetReqTimer()
{
    KillTimer(RETRY_PIC_NET_REQ_TIMERID);
}

void CPicTransServ::OnRetryNetReqTimer()
{
    CMTGurad<CCriticalSect> guard(m_transTaskListLock);
    
    for (auto& it : m_transTaskList)
    {
        CTaskThreadBase::AddTask(it);
    }
    m_transTaskList.clear();
    
    bool bSetTimer = m_transTaskList.empty();
    if (bSetTimer)
        KillRetryNetReqTimer();
}
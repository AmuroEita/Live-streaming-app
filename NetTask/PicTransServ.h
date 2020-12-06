#pragma once

#include <baseclass/TaskThreadBase.h>
#include "PicTransTask.h"

#define RETRY_PIC_NET_REQ_TIMERID   (1006)
#define RETRY_PIC_NET_REQ_TIMEOUT   (2*1000)

class CPicTransServ : public CWindowImpl<CPicTransServ>
                    , public CTaskThreadBase
{
protected:
    CPicTransServ();
    virtual ~CPicTransServ();

    void Init();
    void UnInit();

    virtual void OnTaskCompleted(CTaskItemBase* pTask);
    void AddToRetryList(CPicTransTask* pTask);
    
    
    BEGIN_MSG_MAP(CPicTransTask)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
    END_MSG_MAP()
    
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void SetRetryNetReqTimer();
    void KillRetryNetReqTimer();
    void OnRetryNetReqTimer();

public:
    static CPicTransServ& Inst()
    {
        static CPicTransServ s_inst;
        return s_inst;
    }

    bool TaskIsExist(LPCWSTR lpFilePath);
    void RemoveTaskByPath(LPCWSTR lpFilePath);

protected:
    list<CPicTransTask*> m_transTaskList;
    CCriticalSect	     m_transTaskListLock;
};
#pragma once

#define TimeMgr() CTimeMgr::GetInstance()

class CTimeMgr
	: public pattern::CSingleton<CTimeMgr>
{
public:
    CTimeMgr() : m_lLoginTime_Server_UTC(0), m_dwLoginTime_Local_UTC(0) {};
    ~CTimeMgr() {};

public:
    void SetTime_UTC(UINT64 time_utc)
    {
        m_lLoginTime_Server_UTC = time_utc;
        m_dwLoginTime_Local_UTC = GetTickCount() / 1000;
    }

    //返回服务器时间
    UINT64 GetCurTime_UTC()
    {
        DWORD tNow;
        tNow = GetTickCount() / 1000;

        return m_lLoginTime_Server_UTC + (tNow - m_dwLoginTime_Local_UTC);
    }

private:
    UINT64 m_lLoginTime_Server_UTC;
    DWORD   m_dwLoginTime_Local_UTC;

};

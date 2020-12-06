#pragma once

#include <baseclass/TaskThreadBase.h>
#include <NetUtil/inc/NetUtil.h>

#define     DEFAULT_TASK_RETRY_COUNT    0
#define     DEFINE_TASK_RETRY_COUNT     2
#define     DEFAULT_TIME_OUT_SECONDS    5

typedef unsigned __int64 USERID;

class CPicTransTask : public CTaskItemBase
{
protected:
    CPicTransTask(int nTryCount = DEFAULT_TASK_RETRY_COUNT, time_t timeout = DEFAULT_TIME_OUT_SECONDS);
    virtual ~CPicTransTask();

    virtual void Cancel() {m_CurlCtrl.bCancel = true;}
    
    int DownToFile(const char* url, const wchar_t* saveAsFilePath, bool isResume = false
        , ProgressCallback* callbackFunc = NULL, void* userData = NULL);

    bool NotifyResult(int nRet, void * lpRet, bool bSync = false);
    static int PicProgressCallback(void* clientp, double dltotal, double dlnow,
        double ultotal, double ulnow) {return 0;}

    static void GetFileNameWithExt(const std::wstring & strSrc, std::string & strDst);
    static void MakeFullUri(const char * urlPath, const char * fileName, std::string & strFullPath);

public:
    struct RspCode{enum{
        OK     = 0,
        Failed = 1
    };};

    UINT& RspCode() {return m_uRspCode;}
    
    bool IsOverTime()
    {
        return false;//暂不记超时

        time_t tSpend = (::GetTickCount() - m_begTime)/1000;
        return tSpend > m_timeout;
    }

    bool NeedRetry(bool decreaseCount = true)
    {
        if (RspCode::OK != RspCode() && GetState() != TaskState::Canceled && !IsOverTime() && m_nTryCount>0)
        {
            if (decreaseCount)
            {
                --m_nTryCount;
            }
            return true;
        }
        return false;
    }

    wstring& GetLocalPath()
    {
        return m_wstrLoacPath;
    }

protected:
    NetUtil*        m_pNet;
    CURLControlInfo m_CurlCtrl;
    UINT        m_uRspCode;
    int		    m_nTryCount;
    time_t	    m_begTime, m_timeout;

    //
    HWND        m_hCBWnd;
    UINT        m_uCBMsg;
    wstring     m_wstrLoacPath;
    wstring     m_wstrURL;
    string      m_strUploadPath;
    string      m_strUploadFileName;
    string      m_strUploadBuf;
};

class CPicUploadTask : CPicTransTask
{
public:
    CPicUploadTask();
    
    static void Upload(const wstring& wstrPath, HWND hCBWnd, UINT uCBMsg, char* pSubPath = COS_ChatPicNodeName);
    static void UploadBuff(const string & strBuff, const string & strUploadFileName, HWND hCBWnd, UINT uCBMsg, char* pSubPath = COS_ChatPicNodeName);
protected:
    virtual void Execute();

private:
    void _AppendPriHead(const char * strKey, const char * strValue);
};

class CPicDownloadTask : public CPicTransTask
{
public:
    struct LPARAM_OnDownloadPic
    {
        wstring wstrPath;
    };

public:
    CPicDownloadTask();

    static void Download(const wstring& wtrFName, HWND hCBWnd, UINT uCBMsg);

protected:
    virtual void Execute();
};

class CMatrailDownloadTask : public CPicTransTask
{
public:
    struct LPARAM_OnDownload
    {
        UINT64     uBookId; //
        wstring wstrPath;
        wstring wstrURL;
        DWORD dwTickStart;
        UINT unTimeSpend;
        UINT unType;
        LPVOID pUserData;
    };
public:
	CMatrailDownloadTask();

	static int Download(const UINT64 uBookId, const UINT unType, const wstring& wstrUrl, const std::vector<wstring> vList, const wstring& wtrFName, HWND hCBWnd, UINT uCBMsg, UINT uCBMsgProgress, LPVOID pUserData = NULL);

    static void CancelDownload(const int nTaskToken);

    static int PicProgressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);

protected:
	virtual void Execute()override;

private:
    UINT64 m_uBookId;
    UINT  m_unType;
    UINT m_uMsgProgress;
    DWORD m_dwTickPostProgressMsg;
    DWORD m_dwTickStart;
    std::vector<wstring> m_vSpareUrl;//备用下载地址
};

struct LPARAM_HeadImgeDownloadTask
{
    USERID  usrId;
    wstring wstrFilePath;

    LPARAM_HeadImgeDownloadTask()
    {
        usrId = 0;
    }
};

class CHeadImgeDownloadTask : public CPicTransTask
{
public:
	CHeadImgeDownloadTask();

	static void Download(const wstring& wtrLocalFName, const wstring& wtrURL
        , const USERID usrid, HWND hCBWnd, UINT uCBMsg);

protected:
	virtual void Execute();

public:
    USERID  m_lUsrId;
};

struct LPARAM_HttpTask
{
	std::vector<char> recvBuffer;
};

class CHttpTask : public CPicTransTask
{
public:
	CHttpTask();

	static void ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg);

protected:
	virtual void Execute();
};

class CHttpGetHeaderTask : public CPicTransTask
{
public:
	CHttpGetHeaderTask();

	static void ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg, std::list<std::string>& header);

protected:
	virtual void Execute();

public:
	std::list<std::string> m_headerData;
};

class CHttpPostTask : public CPicTransTask
{
public:
	CHttpPostTask();

    static void ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg,
        std::map<std::string, std::string>& map);

protected:
	virtual void Execute();

public:
	std::map<std::string, std::string> m_mapData;
};

class CHttpSimplePostTask : public CPicTransTask
{
public:
	CHttpSimplePostTask();

	static void ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg,
		std::string data, std::list<std::string>& header);

protected:
	virtual void Execute();

public:
	std::string m_bodyData;
	std::list<std::string> m_headerData;
};

class CHttpPostHeaderTask : public CPicTransTask
{
public:
    CHttpPostHeaderTask();

    static void ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg,
        std::list<std::string>& header, std::map<std::string, std::string>& postMap);

protected:
    virtual void Execute();

public:
    std::map<std::string, std::string> m_mapData;
    std::list<std::string> m_headerData;
};
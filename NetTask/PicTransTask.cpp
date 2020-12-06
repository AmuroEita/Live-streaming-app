#include "stdafx.h"
#include "PicTransTask.h"
#include "PicTransServ.h"
#include "../baseclass/CosAuthSign.h"
#include "../Include/Common.h"
#include "Include/fileHelper.h"

const static int PicTask_Error_InvalidParam = -2;
//
CPicTransTask::CPicTransTask(int nTryCount/* = DEFAULT_TASK_RETRY_COUNT*/, time_t timeout/* = DEFAULT_TIME_OUT_SECONDS*/)
    : m_uRspCode(RspCode::Failed), m_nTryCount(nTryCount)
    , m_begTime(::GetTickCount()), m_timeout(timeout)
{
    m_hCBWnd = NULL;
    m_uCBMsg = 0;
    m_pNet = new NetUtil(L"NetUtil.dll");

}

CPicTransTask::~CPicTransTask()
{
    if (NULL != m_pNet)
    {
        delete m_pNet;
        m_pNet = NULL;
    }
}

int CPicTransTask::DownToFile(const char* url, const wchar_t* saveAsFilePath, bool isResume /*= false */, ProgressCallback* callbackFunc /*= NULL*/, void* userData /*= NULL */)
{
    int nRst = -1;
    if (url == NULL || strlen(url) == 0 || saveAsFilePath==NULL || wcslen(saveAsFilePath)==0)
    {
        //参数错误
        return PicTask_Error_InvalidParam;
    }

    wstring wstrLoacPath = saveAsFilePath;
    if (!NS_FILE_HELPER::PathFileExists(wstrLoacPath.c_str()))
    {
        NS_FILE_HELPER::InsureDirExists(wstrLoacPath.c_str());

        //
        wstring wstrTmpFile = wstrLoacPath + L".tmp";
        LOGPRINTTIMESTAMP(L"DownToFile from URL: %S", url);
        nRst = m_pNet->DownToFile(url, wstrTmpFile.c_str(), isResume, callbackFunc, userData, &m_CurlCtrl);
        if (0 == nRst)
        {
            NS_FILE_HELPER::RenameFile(wstrTmpFile.c_str(), wstrLoacPath.c_str());
        }
        else
        {
            NS_FILE_HELPER::DeleteFile(wstrTmpFile.c_str());
        }
    }
    else
    {
        nRst = 0;
    }
    return nRst;
}

bool CPicTransTask::NotifyResult(int nRet, void * lpRet, bool bSync/* = false*/)
{
    RspCode() = nRet == 0 ? (RspCode::OK) : (RspCode::Failed);
    //参数错误没必要再重试
    if (nRet == PicTask_Error_InvalidParam)
    {
        m_nTryCount = 0;
    }
    if (!NeedRetry(false))
    {
        if (bSync)
        {
            ::PostMessage(m_hCBWnd, m_uCBMsg, (WPARAM)nRet, (LPARAM)lpRet);
        }
        else
        {
            ::SendMessage(m_hCBWnd, m_uCBMsg, (WPARAM)nRet, (LPARAM)lpRet);
        }
        return true;
    }
    return false;
}

void CPicTransTask::GetFileNameWithExt(const std::wstring & strSrc, std::string & strDst)
{
    string strFPath;
    NS_STRING_HELPER::UnicodeToANSI(strSrc, strFPath);
    string strFName, strFExt;
    NS_FILE_HELPER::SplitPathA(strFPath.c_str(), NULL, NULL, &strFName, &strFExt);
    strDst = strFName + strFExt;
}

void CPicTransTask::MakeFullUri(const char * urlPath, const char * fileName, std::string & strFullPath)
{
    strFullPath = "/";
    strFullPath += urlPath;
    strFullPath += "/";
    strFullPath += fileName;
}

//
CPicUploadTask::CPicUploadTask()
{
}

void CPicUploadTask::Upload(const wstring& wstrPath, HWND hCBWnd, UINT uCBMsg, char* pSubPath)
{
    CPicUploadTask* pTask = new CPicUploadTask();
    pTask->m_hCBWnd   = hCBWnd;
    pTask->m_uCBMsg   = uCBMsg;
    pTask->m_wstrLoacPath = wstrPath;
    if(pSubPath)
        pTask->m_strUploadPath = pSubPath;
    CPicTransServ::Inst().AddTask(pTask);
}

void CPicUploadTask::UploadBuff(const string & strBuff, const string & strUploadFileName, HWND hCBWnd, UINT uCBMsg, char* pSubPath /*= COS_ChatPicNodeName*/)
{
    CPicUploadTask* pTask = new CPicUploadTask();
    pTask->m_hCBWnd = hCBWnd;
    pTask->m_uCBMsg = uCBMsg;
    pTask->m_strUploadBuf = strBuff;
    pTask->m_strUploadFileName = strUploadFileName;
    if (pSubPath)
        pTask->m_strUploadPath = pSubPath;
    CPicTransServ::Inst().AddTask(pTask);
}

void CPicUploadTask::Execute()
{
    int nRst = -1;
    do 
    {
        if (!m_pNet->IsOK())
            break;
        
        string strFileName;
        if (m_strUploadFileName.length()>0)
        {
            strFileName = m_strUploadFileName;
        }
        else
        {
            GetFileNameWithExt(m_wstrLoacPath, strFileName);
        }

        string strURI;
        MakeFullUri(m_strUploadPath.c_str(), strFileName.c_str(), strURI);

        string strURL;
//        strURL = MakeBucketUrl();
        strURL += strURI;
        //
        _AppendPriHead("HEAD", strURI.c_str());
        bool bExist = false;
        nRst = m_pNet->HeadRequest(strURL.c_str(), bExist);
        if (bExist)
            break;        
        //
        _AppendPriHead("PUT", strURI.c_str());

        vector<char> vecRst;
        if (m_strUploadBuf.length() > 0)
        {
            nRst = m_pNet->UploadBuffHttpPut(strURL.c_str(), m_strUploadBuf.c_str(), m_strUploadBuf.length(), 
                                    vecRst, NULL, NULL, NULL);
        }
        else
        {
            UINT64 fSize = NS_FILE_HELPER::GetFileSize(m_wstrLoacPath.c_str());
            nRst = m_pNet->UploadFileHttpPut(strURL.c_str(), m_wstrLoacPath.c_str()
                , 0, (unsigned int)fSize, vecRst, NULL, NULL, NULL);
        }

    } while (0);
    
    NotifyResult(nRst, (void*)m_wstrLoacPath.c_str());
}

void CPicUploadTask::_AppendPriHead(const char * strKey, const char * strValue)
{
    //CCosAuthSign cosSign(strKey, strValue);
    string strAuth;
    UINT64 curServerTime = TimeMgr()->GetCurTime_UTC();
  //  cosSign.GetAuthStr((time_t)curServerTime, strAuth);
    NetUtil::HttpHeader httpHeader;
    string strTmp = "Authorization:";
    strTmp += strAuth;
    httpHeader.AppendLine(strTmp.c_str());
    m_pNet->AppendHttpHeader(httpHeader);
}

//
CPicDownloadTask::CPicDownloadTask()
: CPicTransTask(DEFINE_TASK_RETRY_COUNT)
{
}

void CPicDownloadTask::Download(const wstring& wstrPath, HWND hCBWnd, UINT uCBMsg)
{
    CPicTransServ::Inst().RemoveTaskByPath(wstrPath.c_str());

    CPicDownloadTask* pTask = new CPicDownloadTask();
    pTask->m_hCBWnd = hCBWnd;
    pTask->m_uCBMsg = uCBMsg;
    pTask->m_wstrLoacPath = wstrPath;
    CPicTransServ::Inst().AddTask(pTask);
}

void CPicDownloadTask::Execute()
{
    int nRst = -1;
    do 
    {
        if (!m_pNet->IsOK())
            break;

        string strFileName;
        GetFileNameWithExt(m_wstrLoacPath, strFileName);
        string strURI;
        MakeFullUri(COS_ChatPicNodeName, strFileName.c_str(), strURI);

//        string strURL = MakeBucketUrl() + strURI;
        //
//        nRst = DownToFile(strURL.c_str(), m_wstrLoacPath.c_str(), false, NULL, NULL);
        
    } while (0);

    LPARAM_OnDownloadPic pParam;
    pParam.wstrPath = m_wstrLoacPath;

    NotifyResult(nRst, (void*)&pParam);
}

//
CMatrailDownloadTask::CMatrailDownloadTask()
: CPicTransTask(DEFINE_TASK_RETRY_COUNT)
, m_uBookId(0)
, m_uMsgProgress(0)
, m_dwTickPostProgressMsg(0)
, m_unType(0)
{
}

int CMatrailDownloadTask::Download(const UINT64 uBookId, const UINT unType, const wstring& wstrUrl, const std::vector<wstring> vList, const wstring& wtrFName, HWND hCBWnd, UINT uCBMsg, UINT uCBMsgProgress, LPVOID pUserData/* = NULL*/)
{
    CPicTransServ::Inst().RemoveTaskByPath(wtrFName.c_str());

	CMatrailDownloadTask* pTask = new CMatrailDownloadTask();
	pTask->m_hCBWnd   = hCBWnd;
	pTask->m_uCBMsg   = uCBMsg;
	pTask->m_wstrLoacPath = wtrFName;
    pTask->m_wstrURL = wstrUrl;
    pTask->m_uMsgProgress = uCBMsgProgress;
    pTask->m_dwTickPostProgressMsg = GetTickCount();
    pTask->m_dwTickStart = GetTickCount();
    pTask->m_uBookId = uBookId;
	//pTask->m_nTryCount = 0;//构造函数中已设定重试次数
    pTask->m_unType = unType;
    pTask->m_vSpareUrl = vList;
    pTask->UserData() = pUserData;
	return CPicTransServ::Inst().AddTask(pTask);
}

void CMatrailDownloadTask::CancelDownload(const int nTaskToken)
{
    // DownToFile 暂时无法取消
    CPicTransServ::Inst().CancelTask(nTaskToken);
}

int CMatrailDownloadTask::PicProgressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    CMatrailDownloadTask * pTask = (CMatrailDownloadTask*)clientp;
    if (pTask == NULL || pTask->m_hCBWnd == NULL || pTask->m_uMsgProgress == 0)
        return 0;

    UINT iCurTick = GetTickCount();
    if (iCurTick - pTask->m_dwTickPostProgressMsg < 30)
        return 0;

    pTask->m_dwTickPostProgressMsg = iCurTick;

    int iProgress = (dltotal > 0 ? (int)(100 * dlnow / dltotal) : 0);
    PostMessage(pTask->m_hCBWnd, pTask->m_uMsgProgress, (WPARAM)pTask->m_uBookId, iProgress);
    return 0;       // do not return 1, because 1 will abort curl
}

void CMatrailDownloadTask::Execute()
{
	int nRst = -1;
	do
	{
		if (!m_pNet->IsOK())
			break;

        string strUrlA;
        NS_STRING_HELPER::UnicodeToUTF8(m_wstrURL, strUrlA);

        nRst = DownToFile(strUrlA.c_str(), m_wstrLoacPath.c_str(), false, PicProgressCallback, this);
		//使用备用地址尝试
        if (nRst != 0 && m_vSpareUrl.size()>0 && !NeedRetry(false))
        {
            auto it = m_vSpareUrl.begin();
            m_wstrURL = *it;
            // 这个地方是为了和第一个URL次数相同，所以加1
            m_nTryCount = DEFINE_TASK_RETRY_COUNT + 1;
            m_vSpareUrl.erase(it);
        }
	} while (0);

    LPARAM_OnDownload pParam;
    pParam.uBookId = m_uBookId;
    pParam.wstrPath = m_wstrLoacPath;
    pParam.wstrURL = m_wstrURL;
    pParam.dwTickStart = m_dwTickStart;
    pParam.unTimeSpend = GetTickCount() - m_dwTickStart;
    pParam.unType = m_unType;
    pParam.pUserData = UserData();

    NotifyResult(nRst, (void*)&pParam);
}

//
CHeadImgeDownloadTask::CHeadImgeDownloadTask()
    :m_lUsrId(0)
{
}

void CHeadImgeDownloadTask::Download(const wstring& wtrLocalFName, const wstring& wtrURL
    , const USERID usrid, HWND hCBWnd, UINT uCBMsg)
{
	CHeadImgeDownloadTask* pTask = new CHeadImgeDownloadTask();
	pTask->m_hCBWnd   = hCBWnd;
	pTask->m_uCBMsg   = uCBMsg;
	pTask->m_wstrLoacPath = wtrLocalFName;
	pTask->m_wstrURL  = wtrURL;
    pTask->m_lUsrId   = usrid;
	CPicTransServ::Inst().AddTask(pTask);
}

void CHeadImgeDownloadTask::Execute()
{
	int nRst = -1;
	do
	{
		if (!m_pNet->IsOK())
			break;

        string strurl;
        NS_STRING_HELPER::UnicodeToANSI(m_wstrURL, strurl);

        nRst = DownToFile(strurl.c_str(), m_wstrLoacPath.c_str(), false, NULL, NULL);
        
	} while (0);
    
    LPARAM_HeadImgeDownloadTask lpParam;
    lpParam.usrId = m_lUsrId;
    lpParam.wstrFilePath = m_wstrLoacPath;

    NotifyResult(nRst, (void*)&lpParam);
}


//
CHttpTask::CHttpTask()
{
}

void CHttpTask::ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg)
{
	CHttpTask* pTask = new CHttpTask();
	pTask->m_hCBWnd  = hCBWnd;
	pTask->m_uCBMsg  = uCBMsg;
	pTask->m_wstrURL = wtrURL;
	pTask->m_nTryCount = 1;
	CPicTransServ::Inst().AddTask(pTask);
}

void CHttpTask::Execute()
{
	int nRst = -1;
    LPARAM_HttpTask* lpParam = NULL;

	do
	{
		if (!m_pNet->IsOK())
			break;

		string strurl;
		NS_STRING_HELPER::UnicodeToANSI(m_wstrURL, strurl);

        std::vector<char> recvBuffer;		
		nRst = m_pNet->DownToBuffer(strurl.c_str(), recvBuffer);
		
		if (nRst == 0)
		{
			lpParam = new LPARAM_HttpTask();
			lpParam->recvBuffer = recvBuffer;
		}

	} while (0);

    if (!NotifyResult(nRst, (void*)lpParam, true))
    {
        if (lpParam) delete lpParam;
    }
}

//
CHttpPostTask::CHttpPostTask()
{
}

void CHttpPostTask::ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg, std::map<std::string, std::string>& map)
{
	CHttpPostTask* pTask = new CHttpPostTask();
	pTask->m_hCBWnd  = hCBWnd;
	pTask->m_uCBMsg  = uCBMsg;
	pTask->m_wstrURL = wtrURL;
	pTask->m_mapData = map;
	CPicTransServ::Inst().AddTask(pTask);
}

void CHttpPostTask::Execute()
{
	int nRst = -1;
    LPARAM_HttpTask* lpParam = NULL;

	do
	{
		if (!m_pNet->IsOK())
			break;

		string strurl;
		NS_STRING_HELPER::UnicodeToANSI(m_wstrURL, strurl);
		std::vector<char> recvBuffer;
		std::map<std::string, std::string>::const_iterator cstIt = m_mapData.begin();
		NetUtil::PostHelper helper;
		for (; cstIt != m_mapData.end(); ++cstIt)
			helper.Add(cstIt->first.c_str(), 1, cstIt->second.c_str());

		nRst = m_pNet->HttpPostData(strurl.c_str(), helper, recvBuffer);
		
		if (nRst == 0)
		{
			lpParam = new LPARAM_HttpTask();
			lpParam->recvBuffer = recvBuffer;
		}

	} while (0);

    if (!NotifyResult(nRst, (void*)lpParam, true))
    {
        if (lpParam) delete lpParam;
    }
}

//
CHttpPostHeaderTask::CHttpPostHeaderTask()
{
}

void CHttpPostHeaderTask::ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg,
    std::list<std::string>& header, std::map<std::string, std::string>& postData)
{
    CHttpPostHeaderTask* pTask = new CHttpPostHeaderTask();
    pTask->m_hCBWnd = hCBWnd;
    pTask->m_uCBMsg = uCBMsg;
    pTask->m_wstrURL = wtrURL;
    pTask->m_headerData = header;
    pTask->m_mapData = postData;
    CPicTransServ::Inst().AddTask(pTask);
}

void CHttpPostHeaderTask::Execute()
{
    int nRst = -1;
    LPARAM_HttpTask* lpParam = NULL;
    do
    {
        if (!m_pNet->IsOK())
            break;

        string strurl;
        NS_STRING_HELPER::UnicodeToANSI(m_wstrURL, strurl);
        std::vector<char> recvBuffer;
        std::map<std::string, std::string>::const_iterator cstIt = m_mapData.begin();
        NetUtil::PostHelper helper;
        for (; cstIt != m_mapData.end(); ++cstIt)
            helper.Add(cstIt->first.c_str(), 1, cstIt->second.c_str());

        if(m_headerData.size() > 0)
        {
            NetUtil::HttpHeader header;
            for (std::list<std::string>::const_iterator cstIt = m_headerData.begin();
                cstIt != m_headerData.end(); ++ cstIt)
                header.AppendLine(cstIt->c_str());

            m_pNet->AppendHttpHeader(header);
        }

        nRst = m_pNet->HttpPostData(strurl.c_str(), helper, recvBuffer);
        
        if (nRst == 0)
        {
            lpParam = new LPARAM_HttpTask();
            lpParam->recvBuffer = recvBuffer;
        }

    } while (0);

    if (!NotifyResult(nRst, (void*)lpParam, true))
    {
        if (lpParam) delete lpParam;
    }
}

CHttpSimplePostTask::CHttpSimplePostTask()
{

}

void CHttpSimplePostTask::ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg, std::string data, std::list<std::string>& header)
{
	CHttpSimplePostTask* pTask = new CHttpSimplePostTask();
	pTask->m_hCBWnd = hCBWnd;
	pTask->m_uCBMsg = uCBMsg;
	pTask->m_wstrURL = wtrURL;
	pTask->m_bodyData = data;
	pTask->m_headerData = header;

	CPicTransServ::Inst().AddTask(pTask);
}

void CHttpSimplePostTask::Execute()
{
	int nRst = -1;
	LPARAM_HttpTask* lpParam = NULL;

	do
	{
		if (!m_pNet->IsOK())
			break;

		if (m_headerData.size() > 0)
		{
			NetUtil::HttpHeader header;
			for (std::list<std::string>::const_iterator cstIt = m_headerData.begin();
				cstIt != m_headerData.end(); ++cstIt)
				header.AppendLine(cstIt->c_str());

			m_pNet->AppendHttpHeader(header);
		}

		string strurl;
		NS_STRING_HELPER::UnicodeToANSI(m_wstrURL, strurl);
		std::vector<char> recvBuffer;
		nRst = m_pNet->SimpleHttpPostData(strurl.c_str(), m_bodyData.c_str(), recvBuffer);

		if (nRst == 0)
		{
			lpParam = new LPARAM_HttpTask();
			lpParam->recvBuffer = recvBuffer;
		}

	} while (0);

	if (!NotifyResult(nRst, (void*)lpParam, true))
	{
		if (lpParam) delete lpParam;
	}
}

CHttpGetHeaderTask::CHttpGetHeaderTask()
{

}

void CHttpGetHeaderTask::ReqHttp(const wstring& wtrURL, HWND hCBWnd, UINT uCBMsg, std::list<std::string>& header)
{
	CHttpGetHeaderTask* pTask = new CHttpGetHeaderTask();
	pTask->m_hCBWnd = hCBWnd;
	pTask->m_uCBMsg = uCBMsg;
	pTask->m_wstrURL = wtrURL;
	pTask->m_headerData = header;
	CPicTransServ::Inst().AddTask(pTask);
}

void CHttpGetHeaderTask::Execute()
{
	int nRst = -1;
	LPARAM_HttpTask* lpParam = NULL;

	do
	{
		if (!m_pNet->IsOK())
			break;

		string strurl;
		NS_STRING_HELPER::UnicodeToANSI(m_wstrURL, strurl);

		if (m_headerData.size() > 0)
		{
			NetUtil::HttpHeader header;
			for (std::list<std::string>::const_iterator cstIt = m_headerData.begin();
				cstIt != m_headerData.end(); ++cstIt)
				header.AppendLine(cstIt->c_str());

			m_pNet->AppendHttpHeader(header);
		}

		std::vector<char> recvBuffer;
		nRst = m_pNet->DownToBuffer(strurl.c_str(), recvBuffer);

		if (nRst == 0)
		{
			lpParam = new LPARAM_HttpTask();
			lpParam->recvBuffer = recvBuffer;
		}

	} while (0);

	if (!NotifyResult(nRst, (void*)lpParam, true))
	{
		if (lpParam) delete lpParam;
	}
}

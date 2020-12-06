#include "stdafx.h"
#include "LoginUserDataMgr.h"


LoginUserDataMgr::LoginUserDataMgr()
{
	m_strUserName = L"Œ“µƒÍ«≥∆";
	m_strToken = L"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJodHRwOlwvXC93ZWlqaWFuLWFwaS1xYWRybS5tdHEudHZtLmNuXC9hcGlcL3VzZXJcL2xvZ2luIiwiaWF0IjoxNTc1MDcyODI1LCJleHAiOjE1NzU2Nzc2MjUsIm5iZiI6MTU3NTA3MjgyNSwianRpIjoiemZlaWFlZFN2Q0kxQUpRdSIsInN1YiI6ODIsInBydiI6IjIzYmQ1Yzg5NDlmNjAwYWRiMzllNzAxYzQwMDg3MmRiN2E1OTc2ZjciLCJ0eXBlIjoiYXBpIn0.P3HEw3ef5UeiIbqODBZ1T1ILEBJ-pIY-9pCWEEFZB8c";
}


LoginUserDataMgr::~LoginUserDataMgr()
{
}

void LoginUserDataMgr::ParseLogindata(Json::Value& dataNode)
{
	do 
	{
		GetJsonStringUTF16(dataNode, "access_token", m_strToken);
		GetJsonStringUTF16(dataNode, "admin_name", m_strUserName);
		GetJsonStringUTF16(dataNode, "logo", m_strHeadImageURL);
	} while (0);
}

CString LoginUserDataMgr::GetUserName()
{
	return m_strUserName;
}

CString LoginUserDataMgr::GetWBURL()
{
	return m_strHeadImageURL;
}

CString LoginUserDataMgr::GetToken()
{
	return m_strToken;
}

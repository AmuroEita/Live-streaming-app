#pragma once

class LoginUserDataMgr : public pattern::CSingleton<LoginUserDataMgr>
{
public:
	LoginUserDataMgr();
	virtual ~LoginUserDataMgr();

	void ParseLogindata(Json::Value& dataNode);

	CString GetUserName();
	CString GetWBURL();

	CString GetToken();

private:
	CString m_strToken;
	CString m_strUserName;
	CString m_strHeadImageURL;

};


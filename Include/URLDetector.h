#pragma once
#include <string>
#include <list>
using namespace std;
//#include "../CommonUtil/Common.h"

#define COURSE_URL_KEYWORD		L"chuanke.com/"
#define COURSE_URL_KEYWORD_LEN	12

struct URLType {enum {
	Common		= 0,
	Course		= 1,
	VideoCus	= 2,
	SeriesCus	= 3
};};

struct URLCssType {enum {
	Chat		= 0,
	ClassChat	= 1
};};

struct URLItem
{
	UINT	m_uType;
	UINT	m_uCssType;
	UINT64	m_iSID;
	UINT64	m_iCID;
	UINT64	m_iTerm;
	CString	m_strURL;
	wstring	m_wstrGUID;
	
	URLItem() : m_uType(URLType::Common), m_uCssType(URLCssType::Chat)
		, m_iSID(0), m_iCID(0), m_iTerm(0) {}
	URLItem(LPCWSTR lpstrURL)
	{
		m_uType		= URLType::Common;
		m_uCssType	= URLCssType::Chat;
		m_iSID		= 0;
		m_iCID		= 0;
		m_iTerm		= 0;
		if (NULL != lpstrURL)
			m_strURL = lpstrURL;
	}
	//operator LPCWSTR () {return m_strURL;}
	//operator CString& (){return m_strURL;}
	bool IsTokenURL()	{return (URLType::Common != m_uType);}
	bool IsCourseURL()	{return (URLType::Common != m_uType);}
};


struct CURLDetector
{
	static void PeekURL(IN const CString& strText, OUT list<pair<bool, URLItem>>& lstPair)
	{
		PeekURL1(strText, lstPair);
		PeekURL2(lstPair, L"www.");
		// detect url type
		list<pair<bool, URLItem>>::iterator iter = lstPair.begin();
		for (; iter != lstPair.end(); ++iter)
		{
			if (iter->first)
				DetectURLType(iter->second);
		}
	}
	
	static void PeekURL1(IN const CString& strText, OUT list<pair<bool, URLItem>>& lstPair)
	{
		static std::vector<CString> s_arrayProtocol;
		if (true == s_arrayProtocol.empty())
		{
			s_arrayProtocol.push_back(L"https");
			s_arrayProtocol.push_back(L"http");
			s_arrayProtocol.push_back(L"ftp");
		}
		
		int nTotalLen = strText.GetLength();
		int nB = -1, nE = -1, nLastPos = 0;
		while (true)
		{
			nB = strText.Find(L"://", nLastPos);
			if (-1 == nB)
				break;
			int nProtoPos = -1;
			for (size_t i = 0; i < s_arrayProtocol.size(); ++i)
			{
				CString& strItem = s_arrayProtocol.at(i);
				int nItemLen = strItem.GetLength();
				if (nB - nItemLen < 0)
					continue;
				if (strText.Mid(nB - nItemLen, nItemLen) == strItem)
				{
					nProtoPos = nB - nItemLen;
					break;
				}
			}

			if (-1 != nProtoPos)
			{
				nE = nB + 3;
				while (nE < nTotalLen)
				{
					if (IsValidURLChar(strText[nE]))
						++nE;
					else
						break;
				}
				lstPair.push_back(make_pair(false, URLItem(strText.Mid(nLastPos, nProtoPos - nLastPos))));
				// url here
				lstPair.push_back(make_pair(true, URLItem(strText.Mid(nProtoPos, nE - nProtoPos))));
				nLastPos = nE;
			}
			else
			{			
				lstPair.push_back(make_pair(false, URLItem(strText.Mid(nLastPos, nB + 3 - nLastPos))));
				nLastPos = nB + 3;
			}
		}
		if ((-1 != nLastPos) && (nLastPos < nTotalLen))
			lstPair.push_back(make_pair(false, URLItem(strText.Mid(nLastPos))));
	}

	static void PeekURL2(IN OUT list<pair<bool, URLItem>>& lstPair, IN LPCWSTR lpstrURLKey)
	{
		if (NULL == lpstrURLKey)
			return;
		int nURLKeyLen = ::wcslen(lpstrURLKey);
		if (nURLKeyLen <= 0)
			return;

		list<pair<bool, URLItem>>::iterator iter = lstPair.begin();
		for (; iter != lstPair.end(); )
		{
			if (iter->first || iter->second.m_strURL.IsEmpty())
			{
				++iter;
				continue;
			}

			list<pair<bool, URLItem>>::iterator nextIter = iter;
			++nextIter;
			list<pair<bool, URLItem>> pairList2;
			PeekURL2(iter->second.m_strURL, lpstrURLKey, nURLKeyLen, pairList2);
			if (!pairList2.empty())
			{
				list<pair<bool, URLItem>>::iterator iter2 = pairList2.begin();
				for (; iter2 != pairList2.end(); ++iter2)
					lstPair.insert(iter, *iter2);
				lstPair.erase(iter);
			}
			iter = nextIter;
		}
	}

	static void PeekURL2(IN const CString& strText, IN LPCWSTR lpstrURLKey, IN int nURLKeyLen
		, OUT list<pair<bool, URLItem>>& lstPair)
	{
		CString strTextLower = strText;
		strTextLower.MakeLower();
		int nTotalLen = strText.GetLength();
		int nB = -1, nE = -1, nLastPos = 0;
		while (true)
		{
			nB = strTextLower.Find(lpstrURLKey, nLastPos);
			if (-1 == nB)
				break;
			nE = nB + nURLKeyLen;
			while (nE < nTotalLen)
			{
				if (IsValidURLChar(strText[nE]))
					++nE;
				else
					break;
			}
			int nLen = nB - nLastPos;
			if (nLen > 0)
				lstPair.push_back(make_pair(false, URLItem(strText.Mid(nLastPos, nLen))));
			// url here
			lstPair.push_back(make_pair(true, URLItem(strText.Mid(nB, nE - nB))));
			nLastPos = nE;
		}
		if ((-1 != nLastPos) && (nLastPos < nTotalLen))
			lstPair.push_back(make_pair(false, URLItem(strText.Mid(nLastPos))));
	}
	
	inline static bool IsValidURLChar(wchar_t ch)
	{
		if ((ch >= 0x21 && ch <= 0x5a) 
			|| (ch == L'\\') 
			|| (ch >= 0x5e && ch <= 0x7a))
			return true;
		return false;
	}
	
	//#define COURSE_URL_KEYWORD		L"www.chuanke.com/"
	//#define COURSE_URL_KEYWORD_LEN	16
	//Course : http://www.chuanke.com/{$sid}-{$courseid}.html
	//直播课程: http://www.chuanke.com/s{$sid}-{$cid}.html
	//		   http://www.chuanke.com/s{$sid}-{$cid}-{$ts}.html
	//视频课程: http://www.chuanke.com/v{$cid}.html
	//系列课程：http://www.chuanke.com/zhuanji-2-{$cid}-{$page}.html
	static void DetectURLType(URLItem& ui)
	{
		if (ui.m_strURL.IsEmpty())
			return;
		CString strTmp = ui.m_strURL;
		strTmp.MakeLower();
		int nPos = strTmp.Find(COURSE_URL_KEYWORD);
		if (-1 == nPos)
			return;
		LPCWSTR lpstr = strTmp;
		lpstr += (nPos + COURSE_URL_KEYWORD_LEN);
		// course
		ui.m_iSID = ui.m_iCID = 0;
		swscanf_s(lpstr, L"%I64d-%I64d-%I64d", &ui.m_iSID, &ui.m_iCID, &ui.m_iTerm);
		if ((0 != ui.m_iSID) && (0 != ui.m_iCID))
		{
			ui.m_uType = URLType::Course;
			NS_COMMON::GenerateGUID(ui.m_wstrGUID);
			return;
		}
		///
		return;
		///
		// video course
		ui.m_iCID = 0;
		swscanf_s(lpstr, L"v%I64d.", &ui.m_iCID);
		if (0 != ui.m_iCID)
		{
			ui.m_uType = URLType::VideoCus;
			NS_COMMON::GenerateGUID(ui.m_wstrGUID);
			return;
		}
		// series course
		ui.m_iCID = 0;
		swscanf_s(lpstr, L"zhuanji-2-%I64d-%I64d", &ui.m_iCID, &ui.m_iTerm);
		if (0 != ui.m_iCID)
		{
			ui.m_uType = URLType::SeriesCus;
			NS_COMMON::GenerateGUID(ui.m_wstrGUID);
			return;
		}
	}
};

#pragma once
#include <string>
#include <list>
using namespace std;
#include "clipboard.h"

#define IMG_PREFIX_A		"[Image:{"
#define IMG_POSTFIX_A	"}]"

struct CXDictIMRTFItem
{
	enum
	{
		Unknown	= 0x00,
		Text	= 0x01,
		Emote	= 0x02,
		Image	= 0x04
	};
	
	UINT	m_uType;
	int		m_nEmoteID;
	CString	m_strText;
	CString	m_strImgMD5;
	
	CXDictIMRTFItem(UINT uType, int nEmoteID) : m_uType(uType), m_nEmoteID(nEmoteID) {}
	CXDictIMRTFItem(UINT uType, const wchar_t* pText) : m_uType(uType) {if (NULL != pText) m_strText = pText;}
	CXDictIMRTFItem(UINT uType, const wchar_t* pText, const wchar_t* pImgMD5) : m_uType(uType)
	{
		if (NULL != pText) m_strText = pText;
		if (NULL != pImgMD5) m_strImgMD5 = pImgMD5;
	}
};

typedef list<CXDictIMRTFItem> rtfitem_list;
typedef rtfitem_list::iterator rtf_iterator;


struct CXDictIMRTFParser
{
	inline static UINT GetXDictIMRTFID()
	{
		static UINT s_uXDictIMRTF = 0;
		if (0 == s_uXDictIMRTF)
			s_uXDictIMRTF = CClipboard::RegisterClipboardFormat(L"CHUANKE_RTF");
		return s_uXDictIMRTF;
	}

#define IMG_PREFIX		L"[Image:{"
#define IMG_POSTFIX		L"}]"
#define EMOTE_PREFIX	L"[Emote:{"
#define EMOTE_POSTFIX	L"}]"
	
	static void ParseItem(IN const CString& strText, OUT rtfitem_list& itemList)
	{
		ParseEmoteItem(strText, itemList);
		rtf_iterator iter = itemList.begin();
		for (; iter != itemList.end(); )
		{
			if (CXDictIMRTFItem::Emote == iter->m_uType || iter->m_strText.IsEmpty())
			{
				++iter;
				continue;
			}
			
			rtf_iterator nextIter = iter;
			++nextIter;
			rtfitem_list itemList2;
			ParseImageItem(iter->m_strText, itemList2);
			if (!itemList2.empty())
			{
				//rtf_iterator iter2 = itemList2.begin();
				//for (; iter2 != itemList2.end(); ++iter2)
				//	itemList.insert(iter, *iter2);
				itemList.insert(iter, itemList2.begin(), itemList2.end());
				itemList.erase(iter);
			}
			iter = nextIter;
		}
	}
	
	static void ParseEmoteItem(IN const CString& strText, OUT rtfitem_list& itemList)
	{
		CString strItem;
		int nB = -1, nE = -1, nLast = 0;
		while (true)
		{
			nB = strText.Find(EMOTE_PREFIX, nLast);
			if (-1 == nB)
				break;
			nE = strText.Find(EMOTE_POSTFIX, nB);
			if (-1 == nE)
				break;
			if (nE - nB < 9)
				break;
			int nLen = nB - nLast;
			if (nLen > 0)
			{
				strItem = strText.Mid(nLast, nLen);
				itemList.push_back(CXDictIMRTFItem(CXDictIMRTFItem::Text, strItem));
			}
			strItem = strText.Mid(nB + 8, nE - nB - 8);
			itemList.push_back(CXDictIMRTFItem(CXDictIMRTFItem::Emote, ::_wtoi(strItem)));
			nLast = nE + 2;
		}

		if (nLast < strText.GetLength())
		{
			strItem = strText.Mid(nLast);
			itemList.push_back(CXDictIMRTFItem(CXDictIMRTFItem::Text, strItem));
		}
	}

	static void ParseImageItem(IN const CString& strText, OUT rtfitem_list& itemList)
	{
		CString strItem, strImgMD5;
		int nB = -1, nE = -1, nLast = 0;
		while (true)
		{
			nB = strText.Find(IMG_PREFIX, nLast);
			if (-1 == nB)
				break;
			nE = strText.Find(IMG_POSTFIX, nB);
			if (-1 == nE)
				break;
			if (nE - nB < 41)
				break;
			int nLen = nB - nLast;
			if (nLen > 0)
			{
				strItem = strText.Mid(nLast, nLen);
				itemList.push_back(CXDictIMRTFItem(CXDictIMRTFItem::Text, strItem));
			}
			if (strText[nB + 40] == L'.')
			{
				strImgMD5 = strText.Mid(nB + 8, 32);
				strItem = strText.Mid(nB + 8, nE - nB - 8);
				nLast = nE + 2;
				itemList.push_back(CXDictIMRTFItem(CXDictIMRTFItem::Image, strItem, strImgMD5));
			}
			else
			{
				nLast = nB + 8;
				strItem = strText.Mid(nB, 8);
				itemList.push_back(CXDictIMRTFItem(CXDictIMRTFItem::Text, strItem));
			}
		}
		
		if (nLast < strText.GetLength())
		{
			strItem = strText.Mid(nLast);
			itemList.push_back(CXDictIMRTFItem(CXDictIMRTFItem::Text, strItem));
		}
	}
};
/*************************************************************************
/*
/* Operate the Clipboard
/*
/************************************************************************/
#pragma once

#include <assert.h>
#include <Windows.h>
#include <shlobj.h>
#include <vector>
#include "../../../baseclass/StringHelper.h"


class CClipboard
{
#define assert_false_return_false	\
	{assert(false); return false;}

public:
	static bool GetTextFromClipboard(wstring& wstrText, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (FALSE == ::IsClipboardFormatAvailable(CF_UNICODETEXT))
			return false;
		if (FALSE == ::OpenClipboard(hOwnerWnd))
		assert_false_return_false
		HGLOBAL hGlobal = ::GetClipboardData(CF_UNICODETEXT);
		if (NULL == hGlobal)
			goto exit_f1;
		LPVOID pGlobal = ::GlobalLock(hGlobal);
		if (NULL == pGlobal)
			goto exit_f2;		
		size_t siLen = ::GlobalSize(hGlobal);
#define STACK_BUF_SIZE	1024
		if (siLen <= STACK_BUF_SIZE - 2)
		{
			BYTE szBuf[STACK_BUF_SIZE];
			::memcpy(szBuf, pGlobal, siLen);
			szBuf[siLen] = szBuf[siLen + 1] = '\0';
			wstrText = (wchar_t*)szBuf;
		}
		else
		{
			LPBYTE pBuf = (LPBYTE)malloc(siLen + 2);
			if (NULL != pBuf)
			{
				::memcpy(pBuf, pGlobal, siLen);
				pBuf[siLen] = pBuf[siLen + 1] = '\0';
				wstrText = (wchar_t*)pBuf;
				free(pBuf);
				pBuf = NULL;
			}			
		}
		::GlobalUnlock(hGlobal);
		::CloseClipboard();
		return true;

exit_f2:
		::GlobalUnlock(hGlobal);
exit_f1:
		::CloseClipboard();
		assert_false_return_false
	}

	static bool GetTextFromClipboard(string& strText, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		wstring wstrText;
		if (!GetTextFromClipboard(wstrText, hOwnerWnd))
			return false;
		return NS_STRING_HELPER::UnicodeToANSI(wstrText, strText);
	}

	inline static bool SetTextToClipboard(const wchar_t* pText, size_t siLen
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (NULL == pText || siLen <= 0)
		assert_false_return_false;
		siLen = min(siLen, wcslen(pText));
		return _SetTextToClipboard(pText, siLen, bClear, hOwnerWnd);
	}

	inline static bool SetTextToClipboard(const wchar_t* pText
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (NULL == pText)
		assert_false_return_false
		return _SetTextToClipboard(pText, wcslen(pText), bClear, hOwnerWnd);
	}

	inline static bool SetTextToClipboard(const wstring& wstrText
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (wstrText.empty())
		assert_false_return_false
		return _SetTextToClipboard(wstrText.c_str(), wstrText.length(), bClear, hOwnerWnd);
	}

	inline static bool SetTextToClipboard(const char* pText, size_t siLen
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (NULL == pText || siLen <= 0)
		assert_false_return_false
		siLen = min(siLen, strlen(pText));
		return _SetTextToClipboard(pText, siLen, bClear, hOwnerWnd);
	}

	inline static bool SetTextToClipboard(const char* pText
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (NULL == pText)
		assert_false_return_false
		return _SetTextToClipboard(pText, strlen(pText), bClear, hOwnerWnd);
	}

	inline static bool SetTextToClipboard(const string& strText
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (strText.empty())
		assert_false_return_false
		return _SetTextToClipboard(strText.c_str(), strText.length(), bClear, hOwnerWnd);
	}

protected:
	static bool _SetTextToClipboard(const wchar_t* pText, size_t siLen
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		siLen += siLen;
		HGLOBAL hGlobal = ::GlobalAlloc(GHND/*(GMEM_MOVEABLE | GMEM_ZEROINIT)*/, siLen + 2);
		if (NULL == hGlobal)
		assert_false_return_false
		LPVOID pGlobal = ::GlobalLock(hGlobal);
		if (NULL == pGlobal)
			goto exit_f1;
		::memcpy(pGlobal, pText, siLen);
		((BYTE*)pGlobal)[siLen]		= '\0';
		((BYTE*)pGlobal)[siLen + 1]	= '\0';
		::GlobalUnlock(hGlobal);

		if (FALSE == ::OpenClipboard(hOwnerWnd))
			goto exit_f1;
		if (bClear)
			::EmptyClipboard();
		::SetClipboardData(CF_UNICODETEXT, hGlobal);
		::CloseClipboard();
		return true;

exit_f1:
		if (NULL != pGlobal)
			::GlobalFree(hGlobal);
		assert_false_return_false
	}

	static bool _SetTextToClipboard(const char* pText, size_t siLen
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		HGLOBAL hGlobal = ::GlobalAlloc(GHND/*(GMEM_MOVEABLE | GMEM_ZEROINIT)*/, siLen + 1);
		if (NULL == hGlobal)
		assert_false_return_false
		LPVOID pGlobal = ::GlobalLock(hGlobal);
		if (NULL == pGlobal)
			goto exit_f1;
		::memcpy(pGlobal, pText, siLen);
		((BYTE*)pGlobal)[siLen] = '\0';
		::GlobalUnlock(hGlobal);

		if (FALSE == ::OpenClipboard(hOwnerWnd))
			goto exit_f1;
		if (bClear)
			::EmptyClipboard();
		::SetClipboardData(CF_TEXT, hGlobal);
		::CloseClipboard();		
		return true;

exit_f1:
		if (NULL != pGlobal)
			::GlobalFree(hGlobal);
		assert_false_return_false
	}

public:
	inline static bool SetDDBToClipboard(HBITMAP& hBitmap
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (!_SetHandleToClipboard(hBitmap, CF_BITMAP, false, bClear, hOwnerWnd))
			return false;
		hBitmap = NULL;
		return true;
	}

	static bool GetDDBFromClipboard(HBITMAP& hBitmap, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (FALSE == ::IsClipboardFormatAvailable(CF_BITMAP))
			return false;
		if (FALSE == ::OpenClipboard(hOwnerWnd))
		assert_false_return_false
		HBITMAP hBmp = (HBITMAP)::GetClipboardData(CF_BITMAP);
		if (NULL == hBmp)
		{
			::CloseClipboard();
			assert_false_return_false
		}

		BITMAP bmp = {0};
		::GetObject(hBmp, sizeof(bmp), &bmp);
		HDC hDC = ::GetDC(hOwnerWnd);
		HDC hBmpDC = ::CreateCompatibleDC(hDC);
		HDC hBitmapDC = ::CreateCompatibleDC(hDC);
		hBitmap = ::CreateCompatibleBitmap(hDC, bmp.bmWidth, bmp.bmHeight);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hBmpDC, hBmp);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hBitmap);
		::BitBlt(hBitmapDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hBmpDC, 0, 0, SRCCOPY);
		::SelectObject(hBmpDC, hOldBmp);
		::SelectObject(hBitmapDC, hOldBitmap);
		::ReleaseDC(hOwnerWnd, hDC);
		::DeleteObject(hBmpDC);
		::DeleteObject(hBitmapDC);
		
		::CloseClipboard();
		return true;
	}

public:
	static bool SetDropFileListToClipboard(const vector<wstring>& vecFile
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (vecFile.empty())
		assert_false_return_false

		DROPFILES df;
		df.pFiles	= sizeof(df);
		df.pt.x		= 0;
		df.pt.y		= 0;
		df.fNC		= FALSE;
		df.fWide	= TRUE;
		size_t iByteLen = sizeof(df);
		for (size_t i = 0; i < vecFile.size(); ++i)
			iByteLen += (vecFile[i].size() + 1) * sizeof(wchar_t);
		iByteLen += sizeof(wchar_t);
		LPBYTE pByte = (LPBYTE)malloc(iByteLen);
		if (NULL == pByte)
		assert_false_return_false
		LPBYTE pCur = pByte;
		::memcpy(pCur, (LPBYTE)&df, sizeof(df));
		pCur += sizeof(df);
		for (size_t i = 0; i < vecFile.size(); ++i)
		{
			size_t iLen = (vecFile[i].size() + 1) * sizeof(wchar_t);
			::memcpy(pCur, (LPBYTE)(vecFile[i].c_str()), iLen);
			pCur += iLen;
		}
		::memset(pCur, 0, sizeof(wchar_t));
		
		bool bRet = _SetByteToClipboard(pByte, iByteLen, CF_HDROP, bClear, hOwnerWnd);
		::free(pByte);
		return bRet;
	}

	static bool GetDropFileListFromClipboard(OUT vector<wstring>& vecFile, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (FALSE == ::IsClipboardFormatAvailable(CF_HDROP))
			return false;
		if (FALSE == ::OpenClipboard(hOwnerWnd))
		assert_false_return_false
		HDROP hDrop = (HDROP)::GetClipboardData(CF_HDROP);
		if (NULL == hDrop)
		{
			::CloseClipboard();
			assert_false_return_false
		}
		bool bRet = GetDropFileListFromHDrop(hDrop, vecFile);
		//::DragFinish(hDrop);
		::CloseClipboard();
		return bRet;
	}

	static bool GetDropFileListFromHDrop(IN HDROP hDrop, OUT vector<wstring>& vecFile)
	{
		if (NULL == hDrop)
		assert_false_return_false
		UINT uCount = ::DragQueryFile(hDrop, -1, NULL, 0);
		if (0 == uCount)
			return true;
		wchar_t szBuf[MAX_PATH];
		for (UINT i = 0; i < uCount; ++i)
		{
			UINT uRet = ::DragQueryFile(hDrop, i, szBuf, MAX_PATH);
			szBuf[uRet] = L'\0';
			vecFile.push_back(wstring(szBuf));
		}
		return true;
	}

public:
	inline static UINT RegisterClipboardFormat(LPCTSTR lpszFormat) {return ::RegisterClipboardFormat(lpszFormat);}
	
	inline static bool SetCustomeFormatDataToClipboard(UINT uFormat, const wchar_t* pTextData
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (NULL == pTextData)
		{assert(false); return false;}
		return _SetByteToClipboard((LPBYTE)pTextData, (wcslen(pTextData) + 1) * sizeof(wchar_t)
			, uFormat, bClear, hOwnerWnd);
	}

	static bool GetCustomeFormateDataFromClipboard(UINT uFormat, wstring& wstrTextData
		, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		if (FALSE == ::IsClipboardFormatAvailable(uFormat))
			return false;
		if (FALSE == ::OpenClipboard(hOwnerWnd))
		assert_false_return_false
		HGLOBAL hGlobal = ::GetClipboardData(uFormat);
		if (NULL == hGlobal)
			goto exit_f1;
		LPVOID pGlobal = ::GlobalLock(hGlobal);
		if (NULL == pGlobal)
			goto exit_f2;
		size_t siLen = ::GlobalSize(hGlobal);
#define STACK_BUF_SIZE	1024
		if (siLen <= STACK_BUF_SIZE - 2)
		{
			BYTE szBuf[STACK_BUF_SIZE];
			::memcpy(szBuf, pGlobal, siLen);
			szBuf[siLen] = szBuf[siLen + 1] = '\0';
			wstrTextData = (wchar_t*)szBuf;
		}
		else
		{
			LPBYTE pBuf = (LPBYTE)malloc(siLen + 2);
			if (NULL != pBuf)
			{
				::memcpy(pBuf, pGlobal, siLen);
				pBuf[siLen] = pBuf[siLen + 1] = '\0';
				wstrTextData = (wchar_t*)pBuf;
				free(pBuf);
				pBuf = NULL;
			}			
		}
		::GlobalUnlock(hGlobal);
		::CloseClipboard();
		return true;

exit_f2:
		::GlobalUnlock(hGlobal);
exit_f1:
		::CloseClipboard();
		assert_false_return_false
	}

protected:
	static bool _SetHandleToClipboard(HANDLE hHandle, UINT uFormat
		, bool bNewCopy = false, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		HANDLE hGlobal = hHandle;
		if (bNewCopy)
		{
			if (NULL == (hGlobal = _CopyHandle(hHandle)))
			assert_false_return_false
		}

		if (FALSE == ::OpenClipboard(hOwnerWnd))
			goto exit_f1;
		if (bClear)
			::EmptyClipboard();
		::SetClipboardData(uFormat, hGlobal);
		::CloseClipboard();
		return true;

exit_f1:
		if (bNewCopy && (NULL != hGlobal))
			::GlobalFree(hGlobal);
		assert_false_return_false
	}

	static bool _GetHandleFromClipboard(HANDLE& hHandle, UINT uFormat, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		assert_false_return_false
	}

	static bool _SetByteToClipboard(LPBYTE pByte, size_t siLen, UINT uFormat
		, bool bClear = true, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		HGLOBAL hGlobal = ::GlobalAlloc(GHND/*(GMEM_MOVEABLE | GMEM_ZEROINIT)*/, siLen);
		if (NULL == hGlobal)
		assert_false_return_false
		LPVOID pGlobal = ::GlobalLock(hGlobal);
		if (NULL == pGlobal)
			goto exit_f1;
		::memcpy(pGlobal, pByte, siLen);
		::GlobalUnlock(hGlobal);

		if (FALSE == ::OpenClipboard(hOwnerWnd))
			goto exit_f1;
		if (bClear)
			::EmptyClipboard();
		::SetClipboardData(uFormat, hGlobal);
		::CloseClipboard();
		return true;

exit_f1:
		if (NULL != pGlobal)
			::GlobalFree(hGlobal);
		assert_false_return_false
	}

	static bool _GetByteFromClipboard(LPBYTE& pByte, UINT uFormat, HWND hOwnerWnd = ::GetDesktopWindow())
	{
		assert_false_return_false
	}

	static HANDLE _CopyHandle(HANDLE hHandle)
	{
		size_t siLen = ::GlobalSize(hHandle);
		HANDLE hGlobal = ::GlobalAlloc(GHND/*(GMEM_MOVEABLE | GMEM_ZEROINIT)*/, siLen);
		if (NULL == hGlobal)
		assert_false_return_false;
		LPVOID pGlobal = ::GlobalLock(hGlobal);
		if (NULL == pGlobal)
			goto exit_f1;
		LPVOID pHandle = ::GlobalLock(hHandle);
		if (NULL == pHandle)
			goto exit_f2;
		memcpy(pGlobal, pHandle, siLen);
		::GlobalUnlock(hHandle);
		::GlobalUnlock(hGlobal);
		return hGlobal;

exit_f2:
		::GlobalUnlock(hHandle);
exit_f1:
		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
		return NULL;
	}
};


/* reference:
GetClipboardOwner()
IsClipboardFormatAvailable()
OpenClipboard()
GetClipboardData()
SetClipboardData()
EmptyClipboard()
CloseClipboard()

GlobalAlloc()
GlobalFree()
GlobalLock()
GlobalUnLock()
GlobalSize()
GlobalHandle()
*/
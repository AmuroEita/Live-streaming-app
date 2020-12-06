#pragma once
#include <atlimage.h>


struct CDDBUtil
{
	static HBITMAP CreateDDBFromDC(HDC hDC, int nX, int nY, int nWidth, int nHeight)
	{
		if (NULL == hDC)
			return NULL;
		HDC hBmpDC = ::CreateCompatibleDC(hDC);
		if (NULL == hBmpDC)
			return NULL;
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
		if (NULL == hBitmap)
		{
			::DeleteDC(hBmpDC);
			return NULL;
		}
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hBmpDC, hBitmap);
		::BitBlt(hBmpDC, 0, 0, nWidth, nHeight, hDC, nX, nY, SRCCOPY);
		::SelectObject(hBmpDC, hOldBmp);
		::DeleteDC(hBmpDC);
		return hBitmap;
	}
	
	static HBITMAP CreateDDBFromDC(HDC hDC, int nX, int nY, int nWidth, int nHeight
		, int nDstWidth, int nDstHeight)
	{
		if (NULL == hDC)
			return NULL;
		HDC hBmpDC = ::CreateCompatibleDC(hDC);
		if (NULL == hBmpDC)
			return NULL;
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, nDstWidth, nDstHeight);
		if (NULL == hBitmap)
		{
			::DeleteDC(hBmpDC);
			return NULL;
		}
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hBmpDC, hBitmap);
		::SetStretchBltMode(hBmpDC, STRETCH_HALFTONE);
		::StretchBlt(hBmpDC, 0, 0, nDstWidth, nDstHeight, hDC, nX, nY
			, nWidth, nHeight, SRCCOPY);
		::SelectObject(hBmpDC, hOldBmp);
		::DeleteDC(hBmpDC);
		return hBitmap;
	}
	
	static bool CopyDDBToDC(HDC hDC, int nX, int nY, int nWidth, int nHeight
		, HBITMAP hBitmap, int nSrcX, int nSrcY)
	{
		if (NULL == hDC || NULL == hBitmap)
			return false;
		HDC hBmpDC = ::CreateCompatibleDC(hDC);
		if (NULL == hBmpDC)
			return false;
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hBmpDC, hBitmap);
		::BitBlt(hDC, nX, nY, nWidth, nHeight, hBmpDC, nSrcX, nSrcY, SRCCOPY);
		::SelectObject(hBmpDC, hOldBmp);
		::DeleteDC(hBmpDC);
		return true;
	}

	static HBITMAP CopyDDB(HBITMAP hBitmap, HWND hRefWnd = ::GetDesktopWindow())
	{
		if (NULL == hBitmap)
			return NULL;

		BITMAP bmp = {0};
		::GetObject(hBitmap, sizeof(bmp), &bmp);
		HDC hRefDC = ::GetDC(hRefWnd);
		HDC hBmpDC = ::CreateCompatibleDC(hRefDC);
		HDC hBmpDC2 = ::CreateCompatibleDC(hRefDC);
		HBITMAP hBitmap2 = ::CreateCompatibleBitmap(hRefDC, bmp.bmWidth, bmp.bmHeight);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hBmpDC, hBitmap);
		HBITMAP hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC2, hBitmap2);
		::BitBlt(hBmpDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, hBmpDC, 0, 0, SRCCOPY);
		::SelectObject(hBmpDC, hOldBmp);
		::SelectObject(hBmpDC2, hOldBmp2);
		::ReleaseDC(hRefWnd, hRefDC);
		::DeleteObject(hBmpDC);
		::DeleteObject(hBmpDC2);
		return hBitmap2;
	}

	static bool SaveHBitmapToFile(HBITMAP hBitmap, LPCWSTR lpstrFile)
	{
		return false;
// 		if (NULL == hBitmap || NULL == lpstrFile)
// 			return false;
// 		NS_FILE_HELPER::InsureDirExists(lpstrFile);
// 		CImage img;
// 		img.Attach(hBitmap);
// 		HRESULT hR = img.Save(lpstrFile);
// 		img.Detach();
// 		return SUCCEEDED(hR);
	}
};
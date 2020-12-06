#ifndef __CircleImage_H__
#define __CircleImage_H__
#pragma once

class CCircleImageUI : public CLabelUI
{
public:
	CCircleImageUI();
	~CCircleImageUI();
	
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintBkImage(HDC hDC);

protected:
	CDuiString m_sImagePath;
	Gdiplus::Bitmap* m_pImagePathBitmap;
};

#endif // __CircleImage_H__

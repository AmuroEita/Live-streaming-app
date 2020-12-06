#pragma once

class CBrowserTabUI : public COptionUI
{
public:
	CBrowserTabUI();
	virtual ~CBrowserTabUI();

public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void DoEvent(TEventUI& event);

	void DoInit();
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DoPostPaint(HDC hDC, const RECT& rcPaint);
	void PaintStatusImage(HDC hDC);
	void PaintIcon(HDC hDC, const RECT& rcpaint);
	void PaintClose(HDC hDC, const RECT& rcpaint);

	void SetIconSize(SIZE szIcon);
	void SetIconPadding(RECT rcPadding);
	void SetIconImage(LPCTSTR pStrCursor);

	void SetCloseSize(SIZE szIcon);
	void SetClosePadding(RECT rcPadding);
	void SetCloseImage(LPCTSTR pStrCursor);

	void SetCloseHotImage(LPCTSTR pStrCloseHotImage);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	bool TestClose(POINT ptMouse);

protected:
	bool m_bHitClose;
	RECT m_rcOldTextPadding;
	SIZE m_szIcon;
	RECT m_rcIconPadding;
	CDuiString m_sIconImage;

	SIZE m_szClose;
	RECT m_rcClosePadding;
	CDuiString m_sCloseImage;
	CDuiString m_sCloseHotImge;
	POINT m_ptMouse;
	RECT m_rcClose;
	
	UINT m_uCloseButtonState;
};


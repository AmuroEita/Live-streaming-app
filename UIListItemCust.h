#pragma once
class CUIListItemCust : public  CListLabelElementUI
{
public:
	CUIListItemCust();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void DrawItemText(HDC hDC, const RECT& rcItem);

	//void PaintBkImage(HDC hDC);

	LPCTSTR GetIconImge();
	void SetIconImge(LPCTSTR pstrIcon);

private:
	CDuiString m_sSelIconimge;
	CDuiString m_sSelText;

protected:
	CDuiString m_sImagePath;
	Gdiplus::Bitmap* m_pImagePathBitmap;
};
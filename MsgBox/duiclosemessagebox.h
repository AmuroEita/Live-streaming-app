#pragma once
#include "DuiMessageBoxDefine.h"

Dui_Return_Type DuiMsgBox(HWND hwnd, const wchar_t * pwszMsg, const wchar_t * pwszCaption = NULL, const int btnType = DUI_MBOK);

struct MsgboxInfo{
    std::wstring _strTitle;
    int          _nBtnType;
};

class CDuiCloseMessageBox : public WindowImplBase
{
public:
	CDuiCloseMessageBox();
	~CDuiCloseMessageBox();

public://INotifyUI
	virtual void Notify(TNotifyUI& msg);

	//CWindowWndc
protected:
	virtual LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const { return CS_VREDRAW; };

	DuiLib::CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
//     LPCTSTR GetResourceID() const
//     {
//         return MAKEINTRESOURCE(IDR_SKIN);
//    	virtual CDuiString GetZIPFileName() const;

public:
	virtual void InitWindow();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void SetMessage(const wchar_t * pwszMsg, const wchar_t * pwszCaption, const int btnType,const int msgicon = DUI_ICON_CONFIRM);


private:
	CLabelUI  *m_pLabelCaption;
	CLabelUI  *m_pLabelIcon;
    CTextUI   *m_pTextMsgText;
	CButtonUI *m_pBtnClose;
	CButtonUI *m_pBtnOK;
    CButtonUI *m_pBtnCancel;
	CControlUI*m_pGap;
    CHorizontalLayoutUI *m_pBtnContainer;

    RECT m_rcCancelBtnPadding;
};


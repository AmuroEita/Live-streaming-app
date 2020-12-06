#pragma once
#include "DuiMessageBoxDefine.h"


Dui_Return_Type DuiCommMsgBox(HWND hwnd, LPCWSTR lpXmlPath = NULL, const int btnType = DUI_MBOK, LPCWSTR lpTitle = NULL, LPCWSTR lpText = NULL, LPCWSTR lpOk = NULL, LPCWSTR lpCancel = NULL);

class CDuiCommMessageBox : public WindowImplBase
{
public:
    CDuiCommMessageBox(LPCWSTR lpXmlPath);
	~CDuiCommMessageBox();

public://INotifyUI
	virtual void Notify(TNotifyUI& msg);

	//CWindowWndc
protected:
	virtual LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const { return CS_VREDRAW; };

	DuiLib::CDuiString GetSkinFolder();
	DuiLib::CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;

public:
	virtual void InitWindow();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    void SetText(const int btnType, LPCWSTR lpTitle, LPCWSTR lpText, LPCWSTR lpOk, LPCWSTR lpCancel);

private:
    CDuiString m_strXmlPath;

private:
    CLabelUI  *m_pTitleLab;
    CTextUI   *m_pMsgText;
	CButtonUI *m_pCloseBtn;
	CButtonUI *m_pOKBtn;
    CButtonUI *m_pCancelBtn;
};


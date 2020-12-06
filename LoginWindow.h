#pragma once
#define ID_CHNAGEACC_Menu					0	// 切换用户
#define ID_CHNAGEACC_ReLogin				1	// 重新登录
#define ID_CHNAGEACC_TokenInvalid			2	// token失效
#define ID_CHNAGEACC_Kicked					3	// 顶下线
#define ID_CHANGE_PASSWORD					4   // SVC通知修改密码
#define ID_CHNAGEACC_ReLogin_RemoteDbg		5	// 远程调试退出重新登录
#include "CefWndWrapManager.h"
#include "AccListMgr.h"
#include "RoomWindow.h"

class  CMainframe;
class CCircleImageUI;
class CLoginWindow : public WindowImplBase
{
public:
	CLoginWindow(void);
	virtual ~CLoginWindow(void);

	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);

	//IDialogBuilderCallback
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

protected:
	virtual LPCTSTR GetWindowClassName() const { return _T("loginwindow"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };

	DuiLib::CDuiString GetSkinFolder();
	DuiLib::CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;

public:
	void OnMsgReturn(TNotifyUI& msg);
	void OnMsgEnterTab(TNotifyUI& msg);

private:
	void ShowTip(LPCWSTR lpText, bool bErr = true);
	void OnButtonLogin();

	void LoginWithUserNamePwd( LPCWSTR lpacc, LPCWSTR lppw , int nCallBackMsg);
	void LoginFail(LPCWSTR lpcontect);

	void checkUpdate();

	void ExitSAC();

	void InitAccCombox();

	void LoadAccount();
	void SaveAccount(LPCTSTR lpAcc, LPCWSTR lppw);

	//std::string Byte2Hex(char* bytes, int bytelength);

	void HandleThirdpartServerRlt(std::string& strRlt);
	void HandleThirdpartServerRltUpdate(std::string& strRlt);

	bool CheckAccPW();

public:
    // 获取主课表窗体(如果获取不到, 获取的是登录窗口的窗体)
    const HWND GetMainFrameHWND();
 
private:
    //CButtonUI* m_pBtnReflush;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pBackBtn;

	COptionUI* m_pAutoCheck;
	CButtonUI* m_pShowPwd;
	CButtonUI* m_pNotShowPwd;
	CProgressUI* m_pProgress;
	CEditUI* m_pEditAcc;
	CEditUI* m_pEditPwd;
    CEditUI* m_pEditPhone;
    CEditUI* m_pEditVCode;

	CButtonUI* m_pLogin;
	CButtonUI* m_pLoginEn;
    CButtonUI* m_pForgetPwd;
    CButtonUI* m_pBtnLoginPwd;
    CButtonUI* m_pBtnLoginVCode;
    CButtonUI* m_pBtnGetVCode;
    CButtonUI* m_pBtnRegister;

	CVerticalLayoutUI* m_pHorLogin;
	CVerticalLayoutUI* m_pHorProgress;
    CVerticalLayoutUI* m_pVHorPwdInput;
    CVerticalLayoutUI* m_pVHorVCodeInput;
	CLabelUI* m_pLableWelcome;
	CCircleImageUI* m_pAvatarImg;
	CLabelUI* m_pLableErrInfo;
    CLabelUI* m_pLableRightInfo;
	//CLabelUI* m_pLableVersion;
    CLabelUI* m_pLabelNoAcc;

	CComboBoxUI* m_pAccCombox;

	CMainframe* m_pMainWindow;

	CRoomWindow* m_pRoom;

    int     m_iConnectCount;
    int     m_nGetVCodeCount;
    bool    m_bAutoReLogin;      // 是否自动重连
	bool	m_bIsCancel;
    bool    m_bLoginWithPwd;
    CString m_strForgetURL;
    CString m_strRegisterURL;
    CString m_strPhoneURL;
	CAccListMgr m_AccListMgr;

    std::wstring m_wstrURL;
    unsigned int m_unTimeOut;
};
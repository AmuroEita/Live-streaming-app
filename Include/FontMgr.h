#pragma once
#include <Windows.h>
#include <string>
#include <list>
using namespace std; 

#define DEF_FONT_NAME	L"宋体"
#define DEF_FONT_HEIGHT	10
#define DEF_FONT_COLOR	RGB(0, 0, 0)


typedef struct CFontInfo
{
	wstring m_wstrName;
} *PCFontInfo;
typedef list<CFontInfo> fontinfo_list;
typedef fontinfo_list::iterator fontinfo_iterator;


struct CFontOption
{
	CFontOption() : m_wstrFontName(DEF_FONT_NAME), m_rgbColor(DEF_FONT_COLOR), m_nHeight(DEF_FONT_HEIGHT)
		, m_bBold(FALSE), m_bItalic(FALSE), m_bUnderline(FALSE) {}

	COLORREF	m_rgbColor;
	int			m_nHeight;
	BOOL		m_bBold;
	BOOL		m_bItalic;
	BOOL		m_bUnderline;
	wstring		m_wstrFontName;

	static void struct2string(IN const CFontOption& option, OUT wstring& wstr)
	{
#define FONT_OPTION_FORMAT	L"ft=%s|cr=%d|hi=%d|bd=%d|it=%d|ul=%d"
		
		CString strTemp;
		strTemp.Format(FONT_OPTION_FORMAT, option.m_wstrFontName.c_str(), option.m_rgbColor
			, option.m_nHeight, option.m_bBold, option.m_bItalic, option.m_bUnderline);
		wstr = strTemp;
	}

	static void string2struct(IN const wstring& wstr, OUT CFontOption& option)
	{
#define FONT_STRING_FORMAT	L"ft=%[^|]|cr=%[^|]|hi=%[^|]|bd=%[^|]|it=%[^|]|ul=%[^|]"

		wchar_t szFontName[32] = {0}, szColor[32] = {0};
		wchar_t szHeight[8] = {0}, szBold[8] = {0}, szItalic[8] = {0}, szUnderline[8] = {0};
		swscanf_s(wstr.c_str(), FONT_STRING_FORMAT
			, szFontName, 30, szColor, 30, szHeight, 7, szBold, 7, szItalic, 7, szUnderline, 7);
		
		option.m_wstrFontName	= szFontName;
		option.m_rgbColor		= _wtoi(szColor);
		option.m_nHeight		= _wtoi(szHeight);
		option.m_bBold			= _wtoi(szBold);
		option.m_bItalic		= _wtoi(szItalic);
		option.m_bUnderline		= _wtoi(szUnderline);
	}

	static void struct2CharFormat(IN const CFontOption& option, OUT CHARFORMAT& cf)
	{
		::memset(&cf, 0, sizeof(cf));
		cf.cbSize		= sizeof(cf);
		cf.dwMask		= CFM_COLOR | CFM_CHARSET | CFM_FACE | CFM_SIZE 
			| CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
		if (TRUE == option.m_bBold)
			cf.dwEffects |= CFE_BOLD;
		if (TRUE == option.m_bItalic)
			cf.dwEffects |= CFE_ITALIC;
		if (TRUE == option.m_bUnderline)
			cf.dwEffects |= CFE_UNDERLINE;
		cf.yHeight		= option.m_nHeight  * 20;
		cf.crTextColor	= option.m_rgbColor;
		cf.bCharSet		= DEFAULT_CHARSET;
		::wcsncpy_s(cf.szFaceName, LF_FACESIZE, option.m_wstrFontName.c_str()
			, min(LF_FACESIZE - 1, option.m_wstrFontName.size()));
	}
	
	static void string2CharFormat(IN const wstring& wstr, OUT CHARFORMAT& cf)
	{
		CFontOption option;
		string2struct(wstr, option);
		struct2CharFormat(option, cf);
	}
};


class CFontMgr
{
private:
	CFontMgr(bool bInit) : m_bInit(bInit), m_bSaveToDB(false), m_bEnterSend(FALSE)
	{
	}

public:
	fontinfo_list& GetFontList()
	{
		if (!m_fontList.empty())
			return m_fontList;

		// get system font
		HWND hDeskWnd = ::GetDesktopWindow();
		HDC hDC = ::GetDC(hDeskWnd);
		::EnumFonts(hDC, NULL, (FONTENUMPROC)EnumFontsProc, (LPARAM)&m_fontList);
		::ReleaseDC(hDeskWnd, hDC);
		
		// sort
		fontinfo_list font_ch;
		fontinfo_list font_en;
		fontinfo_list font_ch_a;
		fontinfo_list font_en_a;
		fontinfo_iterator iter = m_fontList.begin();
		for (; iter != m_fontList.end(); ++iter)
		{
			if (iter->m_wstrName.size() < 2)
				continue;
			if (L'@' == iter->m_wstrName[0])
			{
				if (iter->m_wstrName[1] > 128)
					font_ch_a.push_back(*iter);
				else
					font_en_a.push_back(*iter);
			}
			else
			{
				if (iter->m_wstrName[0] > 128)
					font_ch.push_back(*iter);
				else
					font_en.push_back(*iter);
			}
		}
		m_fontList.clear();
#define COPY_FONT_LIST(fontlist)				\
		iter = fontlist.begin();				\
		for (; iter != fontlist.end(); ++iter)	\
			m_fontList.push_back(*iter);
		COPY_FONT_LIST(font_ch)
		COPY_FONT_LIST(font_en)
		COPY_FONT_LIST(font_ch_a)
		COPY_FONT_LIST(font_en_a)
		return m_fontList;
	}
	
	vector<int>& GetFontHeightList()
	{
		if (m_fontHeightList.empty())
		{
			m_fontHeightList.reserve(16);
			for (int i = 8; i <= 22; ++i)
				m_fontHeightList.push_back(i);
		}
		return m_fontHeightList;
	}
	
	inline wstring GetCurFontName()		{return m_font.m_wstrFontName;}
	inline COLORREF GetCurFontColor()	{return m_font.m_rgbColor;}
	inline int GetCurFontHeight()		{return m_font.m_nHeight;}
	inline BOOL CurFontIsBold()			{return m_font.m_bBold;}
	inline BOOL CurFontIsItalic()		{return m_font.m_bItalic;}
	inline BOOL CurFontIsUnderline()	{return m_font.m_bUnderline;}
	inline BOOL IsEnterSend()			{return m_bEnterSend;}
	
	void SetCurFontName(const wchar_t* lpwstrName)
	{
		if (m_font.m_wstrFontName == lpwstrName)
			return;
		m_font.m_wstrFontName = lpwstrName;
		//if (m_bSaveToDB) {
			//if (IChuanKeSetting* pSetting = dynamic_cast<IChuanKeSetting*>(ChuanKe::GetSetting()))
			//	pSetting->SetValueString(m_iFontNameID, lpwstrName);}
		CFontOption::struct2CharFormat(m_font, m_charFormat);
		CFontOption::struct2string(m_font, m_strFont);
	}
	
	void SetCurFontColor(COLORREF color)
	{
		if (m_font.m_rgbColor == color)
			return;
		m_font.m_rgbColor = color;
// 		if (m_bSaveToDB) {
// 			if (IChuanKeSetting* pSetting = dynamic_cast<IChuanKeSetting*>(ChuanKe::GetSetting()))
// 				pSetting->SetValueInt(m_iFontColorID, color);}
		CFontOption::struct2CharFormat(m_font, m_charFormat);
		CFontOption::struct2string(m_font, m_strFont);
	}
	
	void SetCurFontHeight(int nHeight)
	{
		if (m_font.m_nHeight == nHeight)
			return;
		m_font.m_nHeight = nHeight;
// 		if (m_bSaveToDB) {
// 			if (IChuanKeSetting* pSetting = dynamic_cast<IChuanKeSetting*>(ChuanKe::GetSetting()))
// 				pSetting->SetValueInt(m_iFontHeightID, nHeight);}
		CFontOption::struct2CharFormat(m_font, m_charFormat);
		CFontOption::struct2string(m_font, m_strFont);
	}
	
	void SetCurFontBold(BOOL bBold)
	{
		if (m_font.m_bBold == bBold)
			return;
		m_font.m_bBold = bBold;
// 		if (m_bSaveToDB) {
// 			if (IChuanKeSetting* pSetting = dynamic_cast<IChuanKeSetting*>(ChuanKe::GetSetting()))
// 				pSetting->SetValueInt(m_iFontBoldID, bBold);}
		CFontOption::struct2CharFormat(m_font, m_charFormat);
		CFontOption::struct2string(m_font, m_strFont);
	}
	
	void SetCurFontItalic(BOOL bItalic)
	{
		if (m_font.m_bItalic == bItalic)
			return;
		m_font.m_bItalic = bItalic;
// 		if (m_bSaveToDB) {
// 			if (IChuanKeSetting* pSetting = dynamic_cast<IChuanKeSetting*>(ChuanKe::GetSetting()))
// 				pSetting->SetValueInt(m_iFontItalicID, bItalic);}
		CFontOption::struct2CharFormat(m_font, m_charFormat);
		CFontOption::struct2string(m_font, m_strFont);
	}
	
	void SetCurFontUnderline(BOOL bUnderline)
	{
		if (m_font.m_bUnderline == bUnderline)
			return;
		m_font.m_bUnderline = bUnderline;
// 		if (m_bSaveToDB) {
// 			if (IChuanKeSetting* pSetting = dynamic_cast<IChuanKeSetting*>(ChuanKe::GetSetting()))
// 				pSetting->SetValueInt(m_iFontUnderlineID, bUnderline);}
		CFontOption::struct2CharFormat(m_font, m_charFormat);
		CFontOption::struct2string(m_font, m_strFont);
	}
	
	void SetEnterSend(BOOL bEnterSend)
	{
		m_bEnterSend = bEnterSend;
// 		if (m_bSaveToDB) {
// 			if (IChuanKeSetting* pSetting = dynamic_cast<IChuanKeSetting*>(ChuanKe::GetSetting()))
// 				pSetting->SetValueInt(m_iEnterSendID, bEnterSend);}
	}
	
	bool LoadCurFontOption()
	{
		// 这里从数据库加载自己的字体设置
// 		IChuanKeSetting* pSetting = ChuanKe::GetSetting();
// 		if (NULL == pSetting)
// 			return false;
		
// 		m_bEnterSend = pSetting->GetValueInt(m_iEnterSendID);
// 		m_font.m_wstrFontName	= pSetting->GetValueString(m_iFontNameID);
// 		m_font.m_rgbColor		= pSetting->GetValueInt(m_iFontColorID);
// 		m_font.m_nHeight		= pSetting->GetValueInt(m_iFontHeightID);
// 		m_font.m_bBold			= pSetting->GetValueInt(m_iFontBoldID);
// 		m_font.m_bItalic		= pSetting->GetValueInt(m_iFontItalicID);
//		m_font.m_bUnderline		= pSetting->GetValueInt(m_iFontUnderlineID);
		CFontOption::struct2CharFormat(m_font, m_charFormat);
		CFontOption::struct2string(m_font, m_strFont);
		return true;
	}

public:
	static int CALLBACK EnumFontsProc(CONST LOGFONT* lplf, CONST TEXTMETRIC* lptm, DWORD dwType, LPARAM lpData)
	{
		if (NULL == lplf || NULL == lpData)
			return 1;

		fontinfo_list* pFontList = (fontinfo_list*)lpData;

		CFontInfo fontInfo;
		fontInfo.m_wstrName = lplf->lfFaceName;
		if (L'@' == lplf->lfFaceName[0])
			pFontList->push_back(fontInfo);
		else
			pFontList->push_front(fontInfo);
		return 1;
	}
	
	static CFontMgr& ChatFont()
	{
		static CFontMgr sFont(false);
		if (!sFont.m_bInit)
		{
			sFont.m_bInit				= true;
			sFont.m_bSaveToDB			= true;
// 			sFont.m_iEnterSendID		= ENTER_SEND;
// 			sFont.m_iFontNameID			= FONT_NAME;
// 			sFont.m_iFontColorID		= FONT_COLOR;
// 			sFont.m_iFontHeightID		= FONT_HEIGHT;
// 			sFont.m_iFontBoldID			= FONT_BOLD;
// 			sFont.m_iFontItalicID		= FONT_ITALIC;
// 			sFont.m_iFontUnderlineID	= FONT_UNDERLINE;
			sFont.LoadCurFontOption();
		}
		return sFont;
	}
	
	static CFontMgr& TextBoardFont()
	{
		static CFontMgr sFont(false);
		if (!sFont.m_bInit)
		{
			sFont.m_bInit			= true;
			sFont.m_bSaveToDB		= false;
			sFont.m_font.m_nHeight	= 13;
			CFontOption::struct2CharFormat(sFont.m_font, sFont.m_charFormat);
			CFontOption::struct2string(sFont.m_font, sFont.m_strFont);
		}
		return sFont;
	}

	static CFontOption& HostTipFontOption()
	{
		static CFontOption sFontOption;
		sFontOption.m_rgbColor = RGB(0, 128, 64);
		return sFontOption;
	}

	static CFontOption& ClientTipFontOption()
	{
		static CFontOption sFontOption;
		sFontOption.m_rgbColor = RGB(0, 0, 255);
		return sFontOption;
	}

	static CFontOption& TeacherTipFontOption()
	{
		static CFontOption sFontOption;
		sFontOption.m_rgbColor = RGB(255, 0, 0);
		return sFontOption;
	}

	static CFontOption& ShortMsgFontOption()
	{
		static CFontOption sFontOption;
		sFontOption.m_rgbColor = RGB(0, 0, 0);
		return sFontOption;
	}

private:
	static fontinfo_list	m_fontList;
	static vector<int>		m_fontHeightList;

	bool		m_bInit, m_bSaveToDB;
	int			m_iEnterSendID, m_iFontNameID, m_iFontColorID
				, m_iFontHeightID, m_iFontBoldID, m_iFontItalicID, m_iFontUnderlineID;
	BOOL		m_bEnterSend;

public:
	wstring		m_strFont;
	CFontOption	m_font;
	CHARFORMAT	m_charFormat;
};

__declspec(selectany) fontinfo_list CFontMgr::m_fontList;
__declspec(selectany) vector<int> CFontMgr::m_fontHeightList;
/**********************************************
* @file        : CUIGdiVideo.h  for  class room
* @description :
* @data        : 2018.12.3
* @author      : liuyanbao
***********************************************/

#pragma  once

class CVerticalLayoutBKUI : public CVerticalLayoutUI
{
   public:
	   CVerticalLayoutBKUI(void);
	virtual ~CVerticalLayoutBKUI(void);

   protected:
       // ---------- CControlUI ----------
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void PaintBkImage(HDC hDC);
};

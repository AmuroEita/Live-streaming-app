#ifndef UIImageAnimate_h__
#define UIImageAnimate_h__

#define DUI_NOTIFY_ANIMATEFINISH L"AnimateFinish"

class  CImageAnimateUI : public CButtonUI
{
public:
	enum
	{ 
		ANIMATE_TIMERID = 1000,
	};

	CImageAnimateUI();
	~CImageAnimateUI();

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	virtual void PaintBkImage(HDC hDC) override;
	virtual void DoEvent(TEventUI& event) override;

	void Start();
	void Stop();

protected:
	void OnTimer( UINT_PTR idEvent );

protected:
	int m_nFrameIndex;
	int m_nFrameCount;
	int m_nFrameTime;
	bool m_bRepeatMode;
};

#endif // UIImageAnimate_h__
#pragma once

class  CDBClickLabelUI : public CLabelUI
{
public:
	CDBClickLabelUI();
	~CDBClickLabelUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void DoEvent(TEventUI& event);
	
};


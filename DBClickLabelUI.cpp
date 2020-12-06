#include "StdAfx.h"
#include "DBClickLabelUI.h"

#include <atlconv.h>

CDBClickLabelUI::CDBClickLabelUI()
{

}

CDBClickLabelUI::~CDBClickLabelUI()
{

}
LPCTSTR CDBClickLabelUI::GetClass() const
{
	return _T("DBClickLabelUI");
}

LPVOID CDBClickLabelUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, L"DBClickLabelUI") == 0) return static_cast<CDBClickLabelUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

void CDBClickLabelUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
		if (m_pParent != NULL) m_pParent->DoEvent(event);
		else CLabelUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_DBLCLICK && IsEnabled()) {
		if (m_pManager != NULL)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_DBCLICK);
		}
	}
	CLabelUI::DoEvent(event);
}

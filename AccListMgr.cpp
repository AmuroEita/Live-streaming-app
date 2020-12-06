#include "stdafx.h"
#include "AccListMgr.h"
#include <atlenc.h>
#include "../../baseclass/AESDecoder.h"
#include "../../baseclass/AESPWD.h"
#include "Base.h"
#include "../../baseclass/PathBase.h"
#include <memory>

CAccListMgr::CAccListMgr(void)
{
}

CAccListMgr::~CAccListMgr(void)
{
	ReleaseList();
}

HRESULT CAccListMgr::LoadAcc(LPCWSTR lpwsFile)
{
	if(NULL == lpwsFile || !PathFileExists(lpwsFile))
		return E_INVALIDARG;

	HANDLE hFile = CreateFile(lpwsFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	ACAccountFileHeader header = {0};
	HRESULT hr = ReadHeader(hFile, &header);
	if(FAILED(hr))
		return hr;

	ReleaseList();

	if(S_OK == hr)
		hr = LoadAcc(hFile, &header);

	CloseHandle(hFile);
	return hr;
}


HRESULT CAccListMgr::LoadAcc(HANDLE hFile, ACAccountFileHeader* lpHeader)
{
	ATLASSERT(INVALID_HANDLE_VALUE != hFile);
	ATLASSERT(lpHeader);

	SetFilePointer(hFile, sizeof(ACAccountFileHeader) + lpHeader->dwOffset, NULL, FILE_BEGIN);

	DWORD dwSize = GetFileSize(hFile, NULL);
	if( dwSize == 0 || dwSize==INVALID_FILE_SIZE) 
		return E_FAIL;

	CAutoVectorPtr<BYTE> spEncBuf;
	spEncBuf.Allocate(dwSize);

	DWORD dwReaded = 0;
	if(!ReadFile(hFile, spEncBuf, dwSize-sizeof(ACAccountFileHeader), &dwReaded, NULL))
		return E_FAIL;

    if (lpHeader &&lpHeader->dwVersion!= SAC_ACCOUNT_FILE_VER)
    {
        return E_FAIL;
    }

	CAESDecoder cd;
	std::unique_ptr<BYTE> spbyBuf;

	DWORD dwDecLen = 0;
	if(!cd.AESDecode((LPBYTE)spEncBuf, dwReaded, AESPWD_KEY, (LPBYTE*)&spbyBuf, &dwDecLen))
		return E_FAIL;

	CDataStreamEx data;
	data.Attach(spbyBuf.get(), dwDecLen, dwDecLen);

	HRESULT hr = E_FAIL;
	do
	{
		DWORD dwItemCount = 0;
		if(!data.GetUINT((UINT&)dwItemCount))
			break;

		DWORD dwAdded = 0;
		for(DWORD i = 0; i < dwItemCount; ++i)
		{
			PACCATTI pItem = new ACCATTI;
			HRESULT hr1 = ParseItem(data, pItem);
			if(SUCCEEDED(hr1))
			{
				m_vecAccList.push_back(pItem);
				++ dwAdded;
			}
			else
			{
				delete pItem;
				ATLASSERT(FALSE);
			}
		}
		if(dwAdded > 0)
		{
			hr = S_OK;
		}
	} while (FALSE);

	data.Detach();	

	return hr ;
}

HRESULT CAccListMgr::ParseItem(CDataStreamEx& data, PACCATTI lpItem)
{
	bool bRtn = CDataStreamEx::ParseStringW(data, lpItem->strAcc);
	bRtn = bRtn && CDataStreamEx::ParseStringW(data, lpItem->strPasswd);
	return bRtn ? S_OK : E_FAIL;
}

HRESULT CAccListMgr::SaveAcc(CDataStreamEx& data)
{
	UINT uCount = (UINT)m_vecAccList.size();
	data.AppendUINT(uCount);

	DWORD dwAdd = 0;
	for(size_t i = 0; i < m_vecAccList.size(); ++i)
	{
		PACCATTI pItem = m_vecAccList.at(i);
		if(pItem)
		{
			UINT uOffset = data.GetPointer();
			bool bRtn = data.AppendTextW(pItem->strAcc);
			bRtn = bRtn && data.AppendTextW(pItem->strPasswd);
			if(!bRtn)
			{
				ATLASSERT(FALSE);
				data.SetPointer(uOffset);
			}
			else
			{
				++dwAdd;
			}
		}
	}
	return dwAdd == (DWORD)m_vecAccList.size() ? S_OK : S_FALSE;
}
//=================================================================================================

HRESULT CAccListMgr::SaveAcc(LPCWSTR lpwsFile)
{
	if(NULL == lpwsFile)
		return E_INVALIDARG;

    CPathHelperBase::MakeDirExist(lpwsFile, true);

	CHandle hFile(CreateFile(lpwsFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
	if(INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	ACAccountFileHeader header = {0};
	header.dwFileFlag = SAC_ACCOUNT_FILE_FLAG;
	header.dwVersion = SAC_ACCOUNT_FILE_VER;

	HRESULT hr = E_FAIL;
	DWORD dwWrited = 0;
	do 
	{
		if(!WriteFile(hFile, &header, sizeof(header), &dwWrited, NULL))
			break;

		CDataStreamEx data;
		hr = SaveAcc(data);
		if(FAILED(hr))
			break;

		LPBYTE pby = NULL;
		DWORD dwEncLen = 0;
		CAESDecoder cd;
		if(!cd.AESEncode(data.GetBuffer(0), data.GetPointer(), AESPWD_KEY, &pby, &dwEncLen))
		{
			hr = E_FAIL;
			break;
		}
		std::unique_ptr<BYTE> spEncBuf(pby);
		if(!WriteFile(hFile, (LPBYTE)spEncBuf.get(), dwEncLen, &dwWrited, NULL))
		{
			hr = E_FAIL;
			break;
		}
	} while (FALSE);

	return hr;
}

HRESULT CAccListMgr::AddAccount(PACCATTI lpAcc)
{
	if(NULL == lpAcc)
		return E_INVALIDARG;

	//todo check data valid

	RemoveAccount(lpAcc->strAcc);
	AccArray::iterator it = m_vecAccList.begin();
	m_vecAccList.insert(it, lpAcc);

	return S_OK;
}

HRESULT CAccListMgr::RemoveAccount(LPCWSTR lpwsAccount)
{
	if(NULL == lpwsAccount)
		return E_INVALIDARG;

	AccArray::iterator it = m_vecAccList.begin();
	for(; it != m_vecAccList.end(); ++it)
	{
		if((*it)->strAcc.CompareNoCase(lpwsAccount) == 0)
		{
			PACCATTI* p = (PACCATTI*)(*it);
			if (p)
			{
				delete p;
				p = NULL;
			}
			m_vecAccList.erase(it);
			break;
		}
	}
	return S_OK;
}

size_t CAccListMgr::GetAccountCount()
{
	return m_vecAccList.size();
}

PACCATTI CAccListMgr::GetAccount(size_t index)
{
	if(m_vecAccList.size() <= index)
		return NULL;

	return m_vecAccList.at(index);
}

PACCATTI CAccListMgr::FindByAccount(LPCWSTR lpwsAccount)
{
	for(size_t i = 0; i < m_vecAccList.size(); ++i)
	{
		PACCATTI pItem = m_vecAccList.at(i);
		if(pItem && pItem->strAcc.CompareNoCase(lpwsAccount) == 0)
		{
			return pItem;
		}
	}
	return NULL;
}

PACCATTI CAccListMgr::FindByAccountPart(LPCWSTR lpwsAccount)
{
	for(size_t i = 0; i < m_vecAccList.size(); ++i)
	{
		PACCATTI pItem = m_vecAccList.at(i);
		if(pItem)
		{
			if(pItem->strAcc.Find(lpwsAccount) == 0)
			return pItem;
		}
	}
	return NULL;
}

//=================================================================================================
HRESULT CAccListMgr::ReadHeader(HANDLE hFile, ACAccountFileHeader* lpHeader)
{
	ATLASSERT(INVALID_HANDLE_VALUE != hFile);

	DWORD dwReaded = 0;
	DWORD dwSizeHigh = 0;
	DWORD dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
	if(dwSizeHigh != 0 || INVALID_FILE_SIZE == dwSizeLow)
	{
		ATLASSERT(FALSE);
		return E_OUTOFMEMORY;
	}

	if(dwSizeLow >= sizeof(DWORD))
	{
		DWORD dwFlag = 0;
		if(!ReadFile(hFile, &dwFlag, sizeof(dwFlag), &dwReaded, NULL))
			return E_FAIL;

		if(dwFlag != SAC_ACCOUNT_FILE_FLAG)
			return S_FALSE;
	}
	else
	{
		ATLASSERT(FALSE);
		return E_FAIL;
	}

	if(dwSizeLow < sizeof(ACAccountFileHeader))
	{
		//Version Check
		return E_FAIL;
	}
	
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	if(!ReadFile(hFile, lpHeader, sizeof(ACAccountFileHeader), &dwReaded, NULL))
		return E_FAIL;

	return S_OK;
}

//=================================================================================================

void CAccListMgr::ReleaseList()
{
	if (m_vecAccList.size() == 0 )
	{
		return;
	}

	for (UINT i=0; i<m_vecAccList.size(); i++)
	{
		PACCATTI pacc = m_vecAccList[i];
		if (pacc)
		{
			delete pacc;
		}
	}

	m_vecAccList.clear();
}

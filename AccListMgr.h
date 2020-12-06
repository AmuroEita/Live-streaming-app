#pragma once
#include <vector>
#include "../../baseclass/DataStream.h"

#define SAC_ACCOUNT_FILE_FLAG			0x41434147

#define SAC_ACCOUNT_FILE_VER			2

struct ACAccountFileHeader
{
	DWORD dwFileFlag;
	DWORD dwVersion;
	DWORD dwReserved;
	DWORD dwOffset;
};

typedef struct _tagAccattri
{
	CString strAcc;
	CString strPasswd;
}ACCATTI, *PACCATTI;

typedef std::vector<PACCATTI> AccArray;

class CAccListMgr
{
public:
	CAccListMgr(void);
	virtual ~CAccListMgr(void);

public:
	HRESULT LoadAcc(LPCWSTR lpwsFile);
	HRESULT SaveAcc(LPCWSTR lpwsFile);

private:
	HRESULT LoadAcc(HANDLE hFile, ACAccountFileHeader* lpHeader);
	HRESULT ParseItem(CDataStreamEx& data, PACCATTI lpItem);

private:
	HRESULT SaveAcc(CDataStreamEx& data);

public:
	HRESULT AddAccount(PACCATTI lpAcc);
	HRESULT RemoveAccount(LPCWSTR lpwsAccount);

	size_t GetAccountCount();
	PACCATTI GetAccount(size_t index);

	PACCATTI FindByAccount(LPCWSTR lpwsAccount);
	PACCATTI FindByAccountPart(LPCWSTR lpwsAccount);

private:
	HRESULT ReadHeader(HANDLE hFile, ACAccountFileHeader* lpHeader);
	void ReleaseList();

private:
	AccArray m_vecAccList;
};


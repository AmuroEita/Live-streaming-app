#pragma once
#include <deque>
using namespace std;
#include "fileHelper.h"

#define TEXTMSG_MAX_BYTES			(15000)
#define TEXTMSG_CHECK_INTERVAL		(5)
#define TEXTMSG_CHECK_INTERVALBYTES	(15000)
#define PIC_CHECK_INTERVAL			(1 * 60)			// 60√Î
#define PIC_CHECK_INTERVALBYTES		(10 * 1024 * 1024)	// 10M


typedef struct CSecBytes
{
	CSecBytes(time_t sec, unsigned int uByte) : m_sec(sec), m_uBytes(uByte) {}
	time_t			m_sec;
	unsigned int	m_uBytes;
} *PCSecBytes;

typedef deque<CSecBytes> secbytes_deque;
typedef secbytes_deque::iterator secbytes_iterator;


class CByteFreqChecker
{
private:
	CByteFreqChecker() {}

public:
	static CByteFreqChecker& Instance()
	{
		static CByteFreqChecker s_instance;
		return s_instance;
	}

public:
	inline bool CheckTextMsgSendFreq(time_t sec, unsigned int uBytes)
	{
		return CheckBytesFreq(sec, uBytes
			, m_txtSecBytesDeq, TEXTMSG_CHECK_INTERVAL, TEXTMSG_CHECK_INTERVALBYTES);
	}
	
	inline bool CheckPicSendFreq(time_t sec, unsigned int uBytes)
	{
		return CheckBytesFreq(sec, uBytes
			, m_picSecBytesDeq, PIC_CHECK_INTERVAL, PIC_CHECK_INTERVALBYTES);
	}

	bool CheckPicSendFreq(time_t sec, const wchar_t* lpstrPicPath)
	{
		if (NULL == lpstrPicPath)
			return false;
		if (!NS_FILE_HELPER::PathFileExists(lpstrPicPath))
			return false;
		ULONGLONG nSize = NS_FILE_HELPER::GetFileSize(lpstrPicPath);
		if (nSize <= 0)
			return true;
		return CheckPicSendFreq(sec, (unsigned int)nSize);
	}

private:
	bool CheckBytesFreq(time_t sec, unsigned int uBytes
		, secbytes_deque& secBytesDeq, time_t interval, unsigned int uMaxBytes)
	{
		unsigned int uTotal = 0;
		secbytes_iterator iter = secBytesDeq.begin();
		for (; iter != secBytesDeq.end(); ++iter)
		{
			if (iter->m_sec + interval < sec)
				break;
			uTotal += iter->m_uBytes;
		}

		if (iter != secBytesDeq.end())
			secBytesDeq.erase(iter, secBytesDeq.end());

		uTotal += uBytes;
		if (uTotal > uMaxBytes)
			return false;
		
		secBytesDeq.push_front(CSecBytes(sec, uBytes));
		return true;
	}

private:
	secbytes_deque m_txtSecBytesDeq;
	secbytes_deque m_picSecBytesDeq;
};
#include "stdafx.h"
#include "uniascii.h"
#include "uslt_cowon.h"


int CUSLTCowon::getSyncLen(BYTE *usltData)
{
	// SYNC Header : 01 01 D7 7B 61 00 [Cnt - 30 Arr] 00 00 00 40 00 01
	// 1 1 215 123 97 0
	if (usltData[27] == 0x00)
		return (int)usltData[28];

	return -1;
}
bool CUSLTCowon::isVaildSyncPos(BYTE *usltData, int usltPos)
{
	// START Part : 08 00 AC [SEC] [MILISEC] [CharLen] -> 6 hex part (bytes)
	if (usltData[usltPos] == 0x08 && usltData[usltPos+1] == 0x00)// && usltData[usltPos+2] == 0xAC) < 종종 AD?
		return true;
	else
		return false;
}
bool CUSLTCowon::getSync(BYTE *usltData, int *timeData, int *usltPos, int usltLen, WCHAR *sync)
{
	int newPos = *usltPos;//getSyncPos(usltData, *usltPos, usltLen);

	// for dumping!
	BYTE dump[255];
	memcpy(dump, usltData + newPos, 255);


	if (!isVaildSyncPos(usltData, newPos))
	{
		// no more Lyrics
		return false;
	} else {
		// 시간 데이터 처리 부분
		*timeData = getSSecFromByte(usltData + newPos + 2);

		// 문자열 처리 부분
		int charlen = (int)usltData[newPos + 5];

		BYTE *pTemp = (usltData + newPos + 6);
		memcpy(sync, pTemp, charlen);
		sync[charlen/2] = '\0';		//2Byte unicode에서는 위치계산시 2byte를 1개로 보기 때문에..

		// usltPos 인덱스 갱신
		*usltPos = newPos + charlen + 6;
		
		return true;
	}

	return false;
}

void CUSLTCowon::CowonUSLT2Lyrics(BYTE *pUData, int pUDatLen, CString *l_lrc, int *l_time, int *l_cnt)
{
	// 3. USLT 헤더에서 가사를 얻어온다
	// SYNC Header : 01 01 D7 7B 61 00 [Cnt - 30 Arr] 00 00 00 40 00 01
	// START Part : 08 00 AC [SEC] [MILISEC] [CharLen] -> 6 hex part (bytes)
	// 70부터 가사 데이터 시작.

	int lyricsCnt = getSyncLen(pUData);
	int sp = 70;//getSyncPos(pUData, 10, pUDatLen);

	//WCHAR을 CString로.. CStringW 써도 돼지만 일단은 ANSI 프로젝트이므로.
	int i;
	char buf[255];
	WCHAR sync[1024];
	for (i=0; i<lyricsCnt; i++)
	{
		getSync(pUData, &l_time[i], &sp, pUDatLen, sync);
		Unicode2ASCII( sync, buf );
		l_lrc[i] = buf;
	}

	*l_cnt = i;
}
void CUSLTCowon::Lyrics2CowonUSLT(BYTE *pUData, int *pUDataLen, CString *l_lrc, int *l_time, int l_cnt)	/* some problem occurs - HC Language */
{
	// init Header Bytes
	for (int i=0; i<70; i++)
		pUData[i] = 0x00;	//NULL
	
	pUData[4] = 0x29;
	pUData[6] = 0x5B;
	memcpy(pUData+7, cowonDevice, strlen(cowonDevice));
	pUData[7+strlen(cowonDevice)] = 0x5D;
	pUData[22] = 0x02;
	pUData[24] = 0xE6;
	pUData[25] = 0x9B;
	pUData[26] = 0x60;
	pUData[28] = (BYTE)l_cnt;	// Count of Lyrics
	pUData[32] = 0x80;
	pUData[34] = 0x06;

	// 70 바이트부터 쓰기 시작
	int sp = 70;
	for (int i=0; i<l_cnt; i++)
	{
		// header part
		pUData[sp] = 0x08;
		pUData[sp+1] = 0x00;
		// time part
		setBYTEFromSsec(pUData + sp + 2, l_time[i]);
		// length part
		WCHAR buf[255];
		wcscpy(buf, l_lrc[i].GetBuffer());
		int lLen = wcslen(buf);
		pUData[sp+5] = (BYTE)lLen*2;

		// data copy part
		memcpy(pUData + sp + 6, buf, lLen*2);

		// reset Index
		sp += lLen*2 + 6;
	}

	*pUDataLen = sp;
}

int CUSLTCowon::getSSecFromByte(BYTE *pSecData)
{
	int nsec, msec, sec;
	// 1. ns에서 0xAC, ms에서 -0x80, sec에서 -0x90 연산 수행
	nsec = (int)(pSecData[0] - 0xAC);	// DEF : AC, MAX : FF, START : AC
	msec = (int)(pSecData[1] - 0x80);	// DEF : F7, MAX : FF, START : 80 -> 빼면 77(119)
	sec = (int)(pSecData[2] - 0x80);	// DEF : 90, MAX : FF, START : 80 -> 빼면 10(16)
	// 2. ms + 128*sec + nsec*16384 - 119 연산 수행 -> 1/100초 단위의 값이 나옴
	return (msec + 128*sec + nsec*16384 - (134 + 0*128));
}
void CUSLTCowon::setBYTEFromSsec(BYTE *pSecData, int ssec)
{
	// 역연산을 수행한다
	int _ssec = ssec + (134 + 0*128);
	int msec = _ssec % 128;
	int sec = (int)(_ssec / 128 % 128);
	int nsec = (int)(_ssec / 16384);
	//int nsec = (int)(_ssec / 16384 * 2);
	pSecData[0] = (BYTE)nsec + 0xAC;
	pSecData[1] = (BYTE)msec + 0x80;
	pSecData[2] = (BYTE)sec + 0x80;
	//if (pSecData[2] < 0x80) pSecData[2] += 0x80;	// exception
}

// DEF Null at : AC 80 80
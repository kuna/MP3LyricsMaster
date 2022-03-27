#include "stdafx.h"
#include "uniascii.h"
#include "uslt_iriver.h"


int CUSLTIriver::getSyncLen(BYTE *usltData)
{
	// SYNC Header : 02 xx xx xx xx ?? ?? 00 00 00 80 00 03
	// xx는 default index, ?? ??은 갯수부분
	// 1 1 215 123 97 0
	for (int i=0; i<60; i++)
	{
		if (usltData[i] == 0x02 && usltData[i+10] == 0x80 && usltData[i+12] == 0x03/* &&
			usltData[i+2] == 0x00 && usltData[i+3] == 0x00 && usltData[i+4] == 0x00*/)
			return (int)(usltData[i+6]);
	}

	return -1;
}
bool CUSLTIriver::isVaildSyncPos(BYTE *usltData, int usltPos)
{
	// START Part : 08 00 AC [SEC] [MILISEC] [CharLen] -> 6 hex part (bytes)
	if (usltData[usltPos] == 0x08 && usltData[usltPos+1] == 0x00)// && usltData[usltPos+2] == 0xAC) < 종종 AD?
		return true;
	else
		return false;
}
bool CUSLTIriver::getSync(BYTE *usltData, int *timeData, int *usltPos, int usltLen, WCHAR *sync)
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
		int ti = getSSecFromByte(usltData + newPos + 2);
		*timeData = ti;

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

void CUSLTIriver::IriverUSLT2Lyrics(BYTE *pUData, int pUDatLen, CString *l_lrc, int *l_time, int *l_cnt)
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
void CUSLTIriver::Lyrics2IriverUSLT(BYTE *pUData, int *pUDataLen, CString *l_lrc, int *l_time, int l_cnt)	/* some problem occurs - HC Language */
{
	// init Header Bytes
	for (int i=0; i<70; i++)
		pUData[i] = 0x00;	//NULL

	pUData[4] = 0x29;
	pUData[6] = 0x5B;
	pUData[7] = 0x33;
	pUData[8] = 0x20;
	pUData[9] = 0x4C;
	pUData[10] = 0x49;
	pUData[11] = 0x4E;
	pUData[12] = 0x45;
	pUData[13] = 0x53;
	pUData[14] = 0x5D;
	pUData[22] = 0x02;
	pUData[23] = 0x00;
	pUData[24] = 0x00;

	pUData[27] = 0x00;
	pUData[28] = (BYTE)l_cnt + 0;

	pUData[32] = 0x80;
	pUData[34] = 0x03;

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

int CUSLTIriver::getSSecFromByte(BYTE *pSecData)
{
	int nsec, msec, sec;
	// 1. 연산수행
	nsec = (int)(pSecData[0] - 0xA0);
	msec = (int)(pSecData[1] - 0x80);
	sec = (int)(pSecData[2] - 0x80);
	// 2. ms + 128*sec + nsec*16384 - 119 연산 수행 -> 1/100초 단위의 값이 나옴
	return (msec + 128*sec + nsec*16384);
}
void CUSLTIriver::setBYTEFromSsec(BYTE *pSecData, int ssec)
{
	// 역연산을 수행한다
	int _ssec = ssec;
	int msec = _ssec % 128;
	int sec = (int)(_ssec / 128 % 128);
	int nsec = (int)(_ssec / 16384);
	pSecData[0] = (BYTE)nsec + 0xA0;
	pSecData[1] = (BYTE)msec + 0x80;
	pSecData[2] = (BYTE)sec + 0x80;
}
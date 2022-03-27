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
	if (usltData[usltPos] == 0x08 && usltData[usltPos+1] == 0x00)// && usltData[usltPos+2] == 0xAC) < ���� AD?
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
		// �ð� ������ ó�� �κ�
		*timeData = getSSecFromByte(usltData + newPos + 2);

		// ���ڿ� ó�� �κ�
		int charlen = (int)usltData[newPos + 5];

		BYTE *pTemp = (usltData + newPos + 6);
		memcpy(sync, pTemp, charlen);
		sync[charlen/2] = '\0';		//2Byte unicode������ ��ġ���� 2byte�� 1���� ���� ������..

		// usltPos �ε��� ����
		*usltPos = newPos + charlen + 6;
		
		return true;
	}

	return false;
}

void CUSLTCowon::CowonUSLT2Lyrics(BYTE *pUData, int pUDatLen, CString *l_lrc, int *l_time, int *l_cnt)
{
	// 3. USLT ������� ���縦 ���´�
	// SYNC Header : 01 01 D7 7B 61 00 [Cnt - 30 Arr] 00 00 00 40 00 01
	// START Part : 08 00 AC [SEC] [MILISEC] [CharLen] -> 6 hex part (bytes)
	// 70���� ���� ������ ����.

	int lyricsCnt = getSyncLen(pUData);
	int sp = 70;//getSyncPos(pUData, 10, pUDatLen);

	//WCHAR�� CString��.. CStringW �ᵵ ������ �ϴ��� ANSI ������Ʈ�̹Ƿ�.
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

	// 70 ����Ʈ���� ���� ����
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
	// 1. ns���� 0xAC, ms���� -0x80, sec���� -0x90 ���� ����
	nsec = (int)(pSecData[0] - 0xAC);	// DEF : AC, MAX : FF, START : AC
	msec = (int)(pSecData[1] - 0x80);	// DEF : F7, MAX : FF, START : 80 -> ���� 77(119)
	sec = (int)(pSecData[2] - 0x80);	// DEF : 90, MAX : FF, START : 80 -> ���� 10(16)
	// 2. ms + 128*sec + nsec*16384 - 119 ���� ���� -> 1/100�� ������ ���� ����
	return (msec + 128*sec + nsec*16384 - (134 + 0*128));
}
void CUSLTCowon::setBYTEFromSsec(BYTE *pSecData, int ssec)
{
	// �������� �����Ѵ�
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
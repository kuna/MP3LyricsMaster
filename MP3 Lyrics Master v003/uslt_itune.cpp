#include "stdafx.h"
#include "uniascii.h"
#include "uslt_itune.h"


void CUSLTITunes::ITunesUSLT2Lyrics(BYTE *pUData, int pUDatLen, CString *l_lrc, int *l_cnt)
{
	int sp = 10;

	//WCHAR을 CString로.. CStringW 써도 돼지만 일단은 ANSI 프로젝트이므로.
	int i,l=0;
	char buf[255];
	WCHAR sync[1024];

	while (sp < pUDatLen)
	{
		bool ch = false;
		for (i=sp; i<pUDatLen; i++) 
		{
			if (pUData[i] == 0x0A && pUData[i+1] == 0x00)
			{
				ch = true;
				break;
			}
		}

		if (!ch) break;

		memcpy(sync, pUData+sp, i-sp);
		sync[(i-sp)/2] = L'\0';
		l_lrc[l] = sync;
		
		sp = i+2;
		l++;
	}

	*l_cnt = l;
}
void CUSLTITunes::Lyrics2ITunesUSLT(BYTE *pUData, int *pUDataLen, CString *l_lrc, int l_cnt)	/* some problem occurs - HC Language */
{
	pUData[0] = 0x01;
	pUData[1] = 'e';
	pUData[2] = 'n';
	pUData[3] = 'g';
	pUData[4] = 0xFF;
	pUData[5] = 0xFE;
	pUData[6] = 0x00;
	pUData[7] = 0x00;
	pUData[8] = 0xFF;
	pUData[9] = 0xFE;

	int sp = 10;
	for (int i=0; i<l_cnt; i++)
	{
		// length part
		WCHAR buf[255];
		wcscpy(buf, l_lrc[i].GetBuffer());
		int lLen = wcslen(buf);
		memcpy(pUData + sp, buf, lLen*2);

		sp += lLen*2;
		pUData[sp] = 0x0A;
		pUData[sp+1] = 0x00;
		sp += 2;
	}

	pUData[sp] = 0x00;
	pUData[sp+1] = 0x00;
	sp += 2;

	*pUDataLen = sp;
}

bool CUSLTITunes::isITunesUSLT(BYTE *pUData, int pUDatLen)
{
	if (pUDatLen < 10) return false;
	if (pUData[0] == 0x01) return true;
	return false;
}
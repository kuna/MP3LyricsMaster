/* samsung uslt <-> lyrics converter */
/* by kuna - kuna.wo.tc */

class CUSLTSamsung
{
public:
	void SamsungUSLT2Lyrics(BYTE *pUData, int pUDatLen, CString *l_lrc, int *l_time, int *l_cnt);	/* not stable - Do Not Use! */
	void Lyrics2SamsungUSLT(BYTE *pUData, int *pUDataLen, CString *l_lrc, int *l_time, int l_cnt);
	int getSyncLen(BYTE *usltData);
private:
	bool isVaildSyncPos(BYTE *usltData, int usltPos);
	bool getSync(BYTE *usltData, int *timeData, int *usltPos, int usltLen, WCHAR *sync);
	int getSSecFromByte(BYTE *pSecData);
	void setBYTEFromSsec(BYTE *pSecData, int ssec);
};
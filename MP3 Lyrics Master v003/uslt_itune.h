/* itunes uslt <-> lyrics converter */
/* by kuna - kuna.wo.tc */

class CUSLTITunes
{
public:
	void ITunesUSLT2Lyrics(BYTE *pUData, int pUDatLen, CString *l_lrc, int *l_cnt);
	void Lyrics2ITunesUSLT(BYTE *pUData, int *pUDataLen, CString *l_lrc, int l_cnt);
	bool isITunesUSLT(BYTE *pUData, int pUDatLen);
};
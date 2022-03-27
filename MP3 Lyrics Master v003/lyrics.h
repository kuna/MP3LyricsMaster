/* by kuna - http://kuna.wo.tc */

#pragma comment(lib,"WSock32.Lib")
#include <afxsock.h>	// for CSocket [Alsong]
#include "afxinet.h"	// for GOMAudio Lyrics Downloader
#include <string>
#define MAXLRC 1024

extern int s_recvSize, s_recvDelay;

class CLyrics
{
public:
	CLyrics();

	bool readLRCFile(WCHAR *filepath);
	bool writeLRCFile(WCHAR *filepath);
	bool writeLRCFileASCII(WCHAR *filepath);

	int getLRCCnt();
	void getLRCHeader(WCHAR *ar, WCHAR *ti, WCHAR *al);
	void setLRCHeader(WCHAR *ar, WCHAR *ti, WCHAR *al);
	bool getLRC(int index, WCHAR *lrc, int *time);
	bool setLRC(int index, WCHAR *lrc, int time);
	void getLRCAll(int *cnt, CString *lrc, int *time, WCHAR *ar, WCHAR *ti, WCHAR *al);
	int getLRCIndex(int time);

	void clearLRC();
	void createLRC(WCHAR *ar, WCHAR *ti, WCHAR *al);
	void addLRC(WCHAR *lrc, int time);
	bool insertLRC(int index, CString lrc, int time);
	bool deleteLRC(int index);

	/* custom function */
	bool getLRCFromGOMServer(WCHAR *hash, WCHAR *title, WCHAR *artist, WCHAR *album);
	void html_entity_decode(WCHAR *str);
	bool getLRCFromAlsongServer(WCHAR *hash);
	bool setTag(WCHAR *data, WCHAR *tag, WCHAR *dPacket);
	bool getLRCFromAlsongServerTA(WCHAR *strTitle, WCHAR *strArtist);

	bool processLyrics(bool ls1, bool ls2, bool ls3, bool depart, bool mp3Mode, bool autoWrap);
private:
public:
	// able to Access.. but don't edit values!
	int l_cnt;
	CString l_lrc[MAXLRC];
	int l_time[MAXLRC];	// 10 milisec

	WCHAR l_ar[255];
	WCHAR l_ti[255];
	WCHAR l_al[255];
};
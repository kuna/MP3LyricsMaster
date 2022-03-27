#ifndef APP_MAIN_KUNA
#define APP_MAIN_KUNA

// 전역변수 몇개는 stdafx.h에 선언했으므로 그쪽에서 참조할것.

// appreances
void toggleViewMode(BOOL isFileList, LPVOID lpWnd);
void toggleEnable(BOOL isEnabled, LPVOID lpWnd);
void clearFList(LPVOID lpWnd);
void clearPList(LPVOID lpWnd);
void addMessage(CString stat, CString msg, LPVOID lpWnd);

void SaveSetting();
void LoadSetting();

// main funcs
UINT MarkMP3Lyrics(LPVOID lParam);
UINT ExtractMP3Lyrics(LPVOID lParam);
UINT ImportMP3Lyrics(LPVOID lParam);
UINT ImportAPIC(LPVOID lParam);
UINT ChangeFrame(LPVOID lParam);

/* global Variables */
extern bool doProcess;
extern int mp3_Machine;
extern bool ls_alsong, ls_gomaudio;
extern bool ls1, ls2, ls3, ls4;
extern int s_recvSize, s_recvDelay;
extern int lrcUnicode;

extern bool b_remark, b_tagautofix, b_tagfind;

extern char _id3Edit_frame[4];
extern WCHAR _id3Edit_val[256];

extern char cowonDevice[256];

struct ITEMDATA {
	WCHAR fileName[256];
	WCHAR folderPath[256];
	int fileSize;
	int lyricsState;
	WCHAR lyricsStr[256];
	int albumCover;
	WCHAR albumPath[256];
	WCHAR s_title[256];
	WCHAR s_artist[256];
	WCHAR s_album[256];
	WCHAR s_genre[256];
	WCHAR s_year[256];
	WCHAR s_comment[256];
	int index;
};

#endif


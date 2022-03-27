// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// MP3 Lyrics Master v003.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"


/* These functions are MUST Declared in stdafx.h [전역변수 - extern] */
bool doProcess;
int mp3_Machine;
bool ls_alsong, ls_gomaudio;
bool ls1, ls2, ls3,ls4;
int s_recvSize, s_recvDelay;
int lrcUnicode;

bool b_remark, b_tagautofix, b_tagfind;

char _id3Edit_frame[4];
WCHAR _id3Edit_val[256];

char cowonDevice[256];
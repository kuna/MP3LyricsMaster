// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// MP3 Lyrics Master v003.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"


/* These functions are MUST Declared in stdafx.h [�������� - extern] */
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
// MP3 Lyrics Master v003.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMP3LyricsMasterv003App:
// �� Ŭ������ ������ ���ؼ��� MP3 Lyrics Master v003.cpp�� �����Ͻʽÿ�.
//

class CMP3LyricsMasterv003App : public CWinApp
{
public:
	CMP3LyricsMasterv003App();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMP3LyricsMasterv003App theApp;
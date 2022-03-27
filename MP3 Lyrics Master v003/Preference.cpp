// Preference.cpp : implementation file
//

#include "stdafx.h"
#include "MP3 Lyrics Master v003.h"
#include "Preference.h"
#include "uniascii.h"

#include "app_Main.h"


// CPreference dialog

IMPLEMENT_DYNAMIC(CPreference, CDialog)

CPreference::CPreference(CWnd* pParent /*=NULL*/)
	: CDialog(CPreference::IDD, pParent)
{

}

CPreference::~CPreference()
{
}

void CPreference::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_ls1);
	DDX_Control(pDX, IDC_CHECK2, m_ls2);
	DDX_Control(pDX, IDC_CHECK3, m_ls3);
	DDX_Control(pDX, IDC_RADIO1, m_val1);
	DDX_Control(pDX, IDC_RADIO2, m_val2);
	DDX_Control(pDX, IDC_RADIO3, m_val3);
	DDX_Control(pDX, IDC_EDIT1, m_cowonModel);
	DDX_Control(pDX, IDC_EDIT2, sample);
	DDX_Control(pDX, IDC_CHECK4, m_lrcUnicode);
	DDX_Control(pDX, IDC_RADIO6, m_val4);
	DDX_Control(pDX, IDC_CHECK5, m_tagAutofix);
	DDX_Control(pDX, IDC_CHECK6, m_remark);
	DDX_Control(pDX, IDC_CHECK7, m_tagfind);
	DDX_Control(pDX, IDC_CHECK8, l_alsong);
	DDX_Control(pDX, IDC_CHECK9, l_gomaudio);
	DDX_Control(pDX, IDC_CHECK10, m_ls4);
}


BEGIN_MESSAGE_MAP(CPreference, CDialog)
	ON_STN_CLICKED(IDC_STATIC12, &CPreference::OnStnClickedStatic12)
	ON_BN_CLICKED(IDC_BUTTON9, &CPreference::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON1, &CPreference::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CPreference::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPreference::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CPreference::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO6, &CPreference::OnBnClickedRadio6)
END_MESSAGE_MAP()


// CPreference message handlers

void CPreference::OnStnClickedStatic12()
{
	// Á¦ÀÛÀÚ È¨ÆäÀÌÁö ¶ç¿ì±â~ ¡Ú
	ShellExecute(NULL, NULL, L"http://kuna.wo.tc", NULL, NULL, SW_SHOW);
}

void CPreference::OnBnClickedButton9()
{
	SendMessage(WM_CLOSE);
}

BOOL CPreference::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (mp3_Machine) {
		case M_SAMSUNG:
			m_val1.SetCheck(1);
			break;
		case M_IRIVER:
			m_val2.SetCheck(1);
			break;
		case M_COWON:
			m_val3.SetCheck(1);
			break;
		case M_ITUNES:
			m_val4.SetCheck(1);
			break;
	}

	l_alsong.SetCheck(ls_alsong);
	l_gomaudio.SetCheck(ls_gomaudio);

	m_lrcUnicode.SetCheck(lrcUnicode);

	m_ls1.SetCheck(ls1);
	m_ls2.SetCheck(ls2);
	m_ls3.SetCheck(ls3);
	m_ls4.SetCheck(!ls4);

	m_remark.SetCheck(b_remark);
	m_tagAutofix.SetCheck(b_tagautofix);
	m_tagfind.SetCheck(b_tagfind);
	
	m_cowonModel.SetWindowTextW(CString(cowonDevice));

	sample.SetWindowTextW(L"[COWON A2] ¸ðµ¨ : COWON A2\r\n[COWON A3/O2] ¸ðµ¨ : COWON A3\r\n[COWON D2/D2+] ¸ðµ¨ : COWON D2\r\n[COWON Q5/P5/V5] ¸ðµ¨ : COWON Q5\r\n[COWON S9/i9/J3] ¸ðµ¨ : COWON S9\r\n[iAUDIO 4/5/U2/G2/G3/F1] ¸ðµ¨ : iAUDIO 4\r\n[iAUDIO F2] ¸ðµ¨ : iAUDIO F2\r\n[iAUDIO M3/M5] ¸ðµ¨ : iAUDIO M3\r\n[iAUDIO T2] ¸ðµ¨ : iAUDIO T2\r\n[iAUDIO U3/i6/i7] ¸ðµ¨ : iAUDIO U3\r\n[iAUDIO U5] ¸ðµ¨ : iAUDIO U5\r\n[iAUDIO X5] ¸ðµ¨ : iAUDIO X5\r\n\r\n\r\nÄÚ¿ø ¸ðµ¨À» »ç¿ëÁßÀÌ½Å °æ¿ì¿£, ÇØ´ç¸ðµ¨¿¡ ¸Â´Â °ªÀ» Á¤È®ÇÏ°Ô ÀÔ·ÂÇÏ¼¼¿ä");

	return TRUE;
}

void CPreference::OnBnClickedButton1()
{
	// ¼³Á¤ ÀúÀå
	if (m_val1.GetCheck()) {
		mp3_Machine = M_SAMSUNG;
	} else if (m_val2.GetCheck()) {
		mp3_Machine = M_IRIVER;
	} else if (m_val3.GetCheck()) {
		mp3_Machine = M_COWON;
	} else if (m_val4.GetCheck()) {
		mp3_Machine = M_ITUNES;
	}

	ls_alsong = l_alsong.GetCheck();
	ls_gomaudio = l_gomaudio.GetCheck();

	lrcUnicode = m_lrcUnicode.GetCheck();

	ls1 = m_ls1.GetCheck();
	ls2 = m_ls2.GetCheck();
	ls3 = m_ls3.GetCheck();
	ls4 = !m_ls4.GetCheck();
	
	b_remark = m_remark.GetCheck();
	b_tagautofix = m_tagAutofix.GetCheck();
	b_tagfind = m_tagfind.GetCheck();

	WCHAR _buf[256];
	m_cowonModel.GetWindowText(_buf, 256);
	Unicode2ASCII(_buf, cowonDevice);

	SaveSetting();
	SendMessage(WM_CLOSE);
//	this->SaveSettings();
}

void CPreference::OnBnClickedRadio1()
{
	m_val1.SetCheck(1);
	m_val2.SetCheck(0);
	m_val3.SetCheck(0);
	m_val4.SetCheck(0);
}

void CPreference::OnBnClickedRadio2()
{
	m_val1.SetCheck(0);
	m_val2.SetCheck(1);
	m_val3.SetCheck(0);
	m_val4.SetCheck(0);
}

void CPreference::OnBnClickedRadio3()
{
	m_val1.SetCheck(0);
	m_val2.SetCheck(0);
	m_val3.SetCheck(1);
	m_val4.SetCheck(0);
}

void CPreference::OnBnClickedRadio6()
{
	m_val1.SetCheck(0);
	m_val2.SetCheck(0);
	m_val3.SetCheck(0);
	m_val4.SetCheck(1);
}

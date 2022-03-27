// LVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MP3 Lyrics Master v003.h"
#include "LVDlg.h"

#include "lyrics.h"
#include "id3.h"
#include "MD5Maker.h"		// create hash

#include "uslt_samsung.h"
#include "uslt_cowon.h"
#include "uslt_iriver.h"
#include "uslt_itune.h"

#include "LRCEdit.h"

extern bool ls_alsong, ls_gomaudio;
extern bool ls1, ls2, ls3, ls4;
extern int lrcUnicode;
extern bool b_tagautofix;

CID3Reader id3;
CLyrics lrc;

CUSLTSamsung u1;
CUSLTCowon u2;
CUSLTIriver u3;
CUSLTITunes u4;

BYTE uData[SMALLBUF];
int uLen;

// LVDlg dialog

IMPLEMENT_DYNAMIC(LVDlg, CDialog)

LVDlg::LVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LVDlg::IDD, pParent)
{

}

LVDlg::~LVDlg()
{
}

void LVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL, m_model);
	DDX_Control(pDX, IDC_LIST1, lv_lrc);
	DDX_Control(pDX, IDC_EDIT6, m_lrcedit);
	DDX_Control(pDX, IDC_EDIT7, m_title);
	DDX_Control(pDX, IDC_EDIT8, m_artist);
}

BOOL LVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	viewMode = false;
	m_lrcedit.ShowWindow(FALSE);
	lv_lrc.ShowWindow(TRUE);

	lv_lrc.InsertColumn(0, L"시간", LVCFMT_LEFT, 120);
	lv_lrc.InsertColumn(1, L"가사", LVCFMT_LEFT, 600);
	
	id3.readMP3File(fPath.GetBuffer());

	WCHAR buf[256];
	id3.getUnicodeData("TIT2", buf);
	m_title.SetWindowTextW(buf);
	id3.getUnicodeData("TPE1", buf);
	m_artist.SetWindowTextW(buf);

	// 리스트에 시간과 파일 추가
	m_model.SetWindowTextW(sModel);
	
	if (iModel > 0) {
		id3.getRawData("USLT", uData, &uLen);
		
		switch (iModel) {
			case M_SAMSUNG:
				u1.SamsungUSLT2Lyrics(uData, uLen, lrc.l_lrc, lrc.l_time, &lrc.l_cnt);
				break;
			case M_COWON:
				u2.CowonUSLT2Lyrics(uData, uLen, lrc.l_lrc, lrc.l_time, &lrc.l_cnt);
				break;
			case M_IRIVER:
				u3.IriverUSLT2Lyrics(uData, uLen, lrc.l_lrc, lrc.l_time, &lrc.l_cnt);
				break;
			case M_ITUNES:
				u4.ITunesUSLT2Lyrics(uData, uLen, lrc.l_lrc, &lrc.l_cnt);
				break;
		}
	}

	resetLrcView();

	return TRUE;
}


BEGIN_MESSAGE_MAP(LVDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON7, &LVDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &LVDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON2, &LVDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &LVDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &LVDlg::OnBnClickedButton5)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &LVDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


void LVDlg::OnBnClickedButton7()
{
	// lyrics to Text
	
	if (!viewMode) {
		CString _tmp;
		for (int i=0; i<lrc.l_cnt; i++) {
			_tmp += lrc.l_lrc[i] + L"\r\n";
		}
		m_lrcedit.SetWindowText(_tmp);

		viewMode = true;
		m_lrcedit.ShowWindow(TRUE);
		lv_lrc.ShowWindow(FALSE);
	} else {
		viewMode = false;
		m_lrcedit.ShowWindow(FALSE);
		lv_lrc.ShowWindow(TRUE);
	}
}


void LVDlg::resetLrcView()
{
	lv_lrc.DeleteAllItems();

	CString _tmp;
	for (int i=0; i<lrc.l_cnt; i++)
	{
		LVITEM lvi;
		::ZeroMemory(&lvi, sizeof(lvi));

		lvi.mask =  LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		_tmp.Format(L"%02d:%02d:%02d", lrc.l_time[i]/6000%60, lrc.l_time[i]/100%60, lrc.l_time[i]%100);
		lvi.pszText = _tmp.GetBuffer();

		int l = lv_lrc.InsertItem(&lvi);
		lv_lrc.SetItemText(l, 1, lrc.l_lrc[i].GetBuffer());
	}
}

void LVDlg::OnBnClickedButton6()
{
	lrc.clearLRC();

	// get Lyrics
	bool res=false;
	CMD5Maker maker;
	int len, offset = id3.mp3DataStartPos;
	WCHAR MD5Hash[255];
	if (!res && ls_alsong) {
		len = H_ALSONG;
		maker.makeMD5AdvanceW(fPath.GetBuffer(), MD5Hash, offset, len);
		res = lrc.getLRCFromAlsongServer(MD5Hash);
	}
	if (!res && ls_gomaudio) {
		len = H_GOMAUDIO;
		maker.makeMD5AdvanceW(fPath.GetBuffer(), MD5Hash, offset, len);
		res = lrc.getLRCFromGOMServer(MD5Hash, L"", L"", L"");
	}
	
	//lrc.processLyrics(ls1, ls2, ls3, ls4, false, false);
	lrc.processLyrics(ls1, ls2, ls3, ls4, true, true);
	resetLrcView();
}

void LVDlg::OnBnClickedButton2()
{
	lrc.clearLRC();
	//


	WCHAR strTitle[256], strArtist[256];
	m_title.GetWindowText(strTitle, 256);
	m_artist.GetWindowText(strArtist, 256);

	bool res;
	res = lrc.getLRCFromAlsongServerTA(strTitle, strArtist);
	
	lrc.processLyrics(ls1, ls2, ls3, ls4, false, false);
	resetLrcView();
}

void LVDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	WCHAR lrcPath[255];
	wcscpy(lrcPath, fPath.GetBuffer());
	int nL = wcslen(lrcPath);
	lrcPath[nL - 3] = 'l';
	lrcPath[nL - 2] = 'r';
	lrcPath[nL - 1] = 'c';

	if (lrcUnicode)
		lrc.writeLRCFile(lrcPath);
	else
		lrc.writeLRCFileASCII(lrcPath);

	CString _tmp;
	_tmp.Format(L"\"%s\"에 저장되었습니다.", lrcPath);
	AfxMessageBox(_tmp);
}


void LVDlg::OnBnClickedButton5()
{
	if (b_tagautofix)
	{
		id3.setUnicodeData("TIT2", lrc.l_ti, lstrlen(lrc.l_ti));
		id3.setUnicodeData("TPE1", lrc.l_ar, lstrlen(lrc.l_ar));
		id3.setUnicodeData("TALB", lrc.l_al, lstrlen(lrc.l_al));
	}
	lrc.processLyrics(ls1, ls2, ls3, ls4, true, true);

	switch (iModel) {
		case M_SAMSUNG:
			u1.Lyrics2SamsungUSLT(uData, &uLen, lrc.l_lrc, lrc.l_time, lrc.l_cnt);
			break;
		case M_COWON:
			u2.Lyrics2CowonUSLT(uData, &uLen, lrc.l_lrc, lrc.l_time, lrc.l_cnt);
			break;
		case M_IRIVER:
			u3.Lyrics2IriverUSLT(uData, &uLen, lrc.l_lrc, lrc.l_time, lrc.l_cnt);
			break;
		case M_ITUNES:
			u4.Lyrics2ITunesUSLT(uData, &uLen, lrc.l_lrc, lrc.l_cnt);
			break;
	}
	
	id3.setRawData("USLT", uData, uLen);
	id3.updateID3Data();
	id3.updateMP3File();

	AfxMessageBox(L"파일 갱신이 완료되었습니다");
}


void LVDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int ni = lv_lrc.GetNextItem(-1, LVNI_SELECTED);
	if (ni < 0) return;

	nowSelInd = ni;

	CLRCEdit leWnd;
	leWnd.m_time = lrc.l_time[nowSelInd];
	leWnd.m_lrc = lrc.l_lrc[nowSelInd];
	if (leWnd.DoModal() == IDOK) {
		lrc.l_time[nowSelInd] = leWnd.m_time;
		lrc.l_lrc[nowSelInd] = leWnd.m_lrc; 
		resetLrcView();
	}

	*pResult = 0;
}

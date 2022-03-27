// MP3 Lyrics Master v003Dlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ID3TagEditor.h"
#include "LVDlg.h"
#include "preference.h"
#include "app_Main.h"

// CMP3LyricsMasterv003Dlg 대화 상자

class CMP3LyricsMasterv003Dlg : public CDialog
{

// 생성입니다.
public:
	CMP3LyricsMasterv003Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MP3LYRICSMASTERV003_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	int checkIsGroupExist(CListCtrl &clc, WCHAR *str);
	int insertGroup(CListCtrl &clc, WCHAR *str);
	bool isDuplicate(WCHAR *filename);
	void insertItem(WCHAR *filename);
	int ind_group;
	int ind_item;

	int frm_width;
	int frm_height;

public:
	void SearchDirectory(CString parm_search_path);
	bool getCurrentData(ITEMDATA *id);
	bool editCurrentData(ITEMDATA *id);
	void insertSelItem2Progress();
	void resetLayout(CRect* wp);

	void loadFrmInfo();
	void saveFrmInfo();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton cb_flist;
public:
	CButton cb_progress;
public:
	CProgressCtrl m_progress;
public:
	CButton b_cancel;
public:
	CButton b_add;
public:
	CListCtrl lb_flist;
public:
	CListCtrl lb_progress;
public:
	CButton m_lyrics;
public:
	CButton m_lrcExport;
public:
	CButton m_lrcImport;
public:
	CButton m_pref;
public:
	CButton m_rename;
public:
	CButton m_editTag;
public:
	afx_msg void OnBnClickedButton7();
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedCheck1();
public:
	afx_msg void OnBnClickedCheck2();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnLbnSelchangeList1();
public:
	afx_msg void OnBnClickedButton17();
public:
	afx_msg void OnBnClickedButton8();
public:
	afx_msg void OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	CButton b_delete;
public:
	afx_msg void OnClose();
public:
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCmTwitpic();
	afx_msg void OnCmLyrics();
	afx_msg void OnCmFolder();
	afx_msg void OnCmRemove();
	afx_msg void OnCmLmdel();
};

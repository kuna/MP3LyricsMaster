#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// LVDlg dialog

class LVDlg : public CDialog
{
	DECLARE_DYNAMIC(LVDlg)

public:
	void resetLrcView();
	bool viewMode;

	int nowSelInd;

public:
	CString fPath;
	CString sModel;
	int iModel;

	LVDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LVDlg();

// Dialog Data
	enum { IDD = IDD_LRCVIEWER };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_model;
public:
	CListCtrl lv_lrc;
public:
	afx_msg void OnStnClickedModel();
public:
	afx_msg void OnBnClickedButton7();
public:
	afx_msg void OnBnClickedButton6();
public:
	CEdit m_lrcedit;
public:
	afx_msg void OnBnClickedButton2();
public:
	CEdit m_title;
public:
	CEdit m_artist;
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};

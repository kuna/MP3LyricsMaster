#pragma once

#include "afxwin.h"

// CPreference dialog

class CPreference : public CDialog
{
	DECLARE_DYNAMIC(CPreference)

public:
	CPreference(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL OnInitDialog();
	virtual ~CPreference();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStatic12();
public:
	afx_msg void OnBnClickedButton9();
public:
	afx_msg void OnBnClickedButton1();
public:
	CButton m_ls1;
public:
	CButton m_ls2;
public:
	CButton m_ls3;
public:
	CButton m_val1;
public:
	CButton m_val2;
public:
	CButton m_val3;
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnBnClickedRadio4();
public:
	afx_msg void OnBnClickedRadio5();
public:
	afx_msg void OnBnClickedRadio3();
public:
	CEdit m_cowonModel;
public:
	CEdit sample;
public:
	CButton m_lrcUnicode;
public:
	CButton m_val4;
public:
	CButton m_tagAutofix;
public:
	CButton m_remark;
public:
	afx_msg void OnBnClickedRadio6();
public:
	CButton m_tagfind;
public:
	CButton l_alsong;
public:
	CButton l_gomaudio;
	CButton m_ls4;
};

#pragma once

#include "id3.h"	// uses ID3 Tag Analyzer
#include "afxwin.h"
#include "app_Main.h"

/* cimage */
#include "atlimage.h"

// CID3TagEditor dialog

class CID3TagEditor : public CDialog
{
	DECLARE_DYNAMIC(CID3TagEditor)

public:
	// calling Func
	bool loadFileID3Tag(CString filename);
	void initCtrl();
	void editMode();

protected:
	CImage cImg;
	WCHAR picPath[256];
	ITEMDATA id;
	virtual BOOL OnInitDialog();

public:
	CID3TagEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CID3TagEditor();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClose();
public:
//	afx_msg void OnNcDestroy();
public:
	CEdit m_filepath;
public:
	CEdit m_title;
public:
	CEdit m_artist;
public:
	CEdit m_album;
public:
	CEdit m_genre;
public:
	CEdit m_year;
public:
	CEdit m_comment;
public:
	CButton b_apicExport;
public:
	CButton b_apicImport;
public:
	CButton m_apply;
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton13();
public:
	afx_msg void OnBnClickedButton14();
public:
	afx_msg void OnBnClickedButton15();
public:
	afx_msg void OnBnClickedButton16();
public:
	CButton m_picpreview;
public:
	afx_msg void OnPaint();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

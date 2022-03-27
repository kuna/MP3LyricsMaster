#pragma once

#include "resource.h"
#include "afxwin.h"

// CTwitPicUpload dialog

class CTwitPicUpload : public CDialogEx
{
	DECLARE_DYNAMIC(CTwitPicUpload)

public:
	CTwitPicUpload(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTwitPicUpload();

	CString picPath;
	CString picText;

// Dialog Data
	enum { IDD = IDD_TWITPIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_message;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
		CEdit m_id;
	CEdit m_pass;
};

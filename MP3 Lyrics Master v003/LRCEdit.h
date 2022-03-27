#pragma once

#include "resource.h"
#include "afxwin.h"


// CLRCEdit dialog

class CLRCEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CLRCEdit)

public:
	CLRCEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLRCEdit();

	int m_time;
	CString m_lrc;

// Dialog Data
	enum { IDD = IDD_LRCEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit e_time;
	CEdit e_lrc;
};

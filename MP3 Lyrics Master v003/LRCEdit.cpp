// LRCEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LRCEdit.h"
#include "afxdialogex.h"


// CLRCEdit dialog

IMPLEMENT_DYNAMIC(CLRCEdit, CDialogEx)

CLRCEdit::CLRCEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLRCEdit::IDD, pParent)
{

}

CLRCEdit::~CLRCEdit()
{
}

void CLRCEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, e_time);
	DDX_Control(pDX, IDC_EDIT2, e_lrc);
}


BEGIN_MESSAGE_MAP(CLRCEdit, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLRCEdit::OnBnClickedOk)
END_MESSAGE_MAP()


// CLRCEdit message handlers

BOOL CLRCEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	WCHAR s_time[255];
	_itow(m_time, s_time, 10);
	
	e_time.SetWindowTextW(s_time);
	e_lrc.SetWindowTextW(m_lrc);

	return FALSE;
}


void CLRCEdit::OnBnClickedOk()
{
	CString s_time;
	e_time.GetWindowText(s_time);
	e_lrc.GetWindowText(m_lrc);
	m_time = _wtoi(s_time);

	CDialogEx::OnOK();
}

// TwitPicUpload.cpp : implementation file
//

#include "stdafx.h"
#include "TwitPicUpload.h"
#include "afxdialogex.h"
#include "twitPic.h"


// CTwitPicUpload dialog

IMPLEMENT_DYNAMIC(CTwitPicUpload, CDialogEx)

CTwitPicUpload::CTwitPicUpload(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTwitPicUpload::IDD, pParent)
{

}

CTwitPicUpload::~CTwitPicUpload()
{
}

void CTwitPicUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_message);
	DDX_Control(pDX, IDC_EDIT2, m_id);
	DDX_Control(pDX, IDC_EDIT3, m_pass);
}


BEGIN_MESSAGE_MAP(CTwitPicUpload, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTwitPicUpload::OnBnClickedOk)
END_MESSAGE_MAP()


// CTwitPicUpload message handlers


BOOL CTwitPicUpload::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	char buf[255];
	GetPrivateProfileStringA( "TWITTER", "id", "", buf, 255, ".\\config.ini");			m_id.SetWindowTextW(CStringW(buf));
	GetPrivateProfileStringA( "TWITTER", "pass", "", buf, 255, ".\\config.ini");		m_pass.SetWindowTextW(CStringW(buf));
	m_message.SetWindowTextW(picText);

	return FALSE;
}


void CTwitPicUpload::OnBnClickedOk()
{
	WCHAR s_id[255], s_pass[255], s_msg[255];
	m_id.GetWindowTextW(s_id, 255);
	m_pass.GetWindowTextW(s_pass, 255);
	m_message.GetWindowTextW(s_msg, 255);
	
	twitPic_UploadW(s_id, s_pass, s_msg, picPath);

	WritePrivateProfileStringA( "TWITTER", "id", CStringA(s_id), ".\\config.ini" );
	WritePrivateProfileStringA( "TWITTER", "pass", CStringA(s_pass), ".\\config.ini" );
	
	CDialogEx::OnOK();
}

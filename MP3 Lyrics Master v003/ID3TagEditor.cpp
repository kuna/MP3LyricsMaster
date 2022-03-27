// ID3TagEditor.cpp : implementation file
//

#include "stdafx.h"
#include "MP3 Lyrics Master v003.h"
#include "MP3 Lyrics Master v003Dlg.h"
#include "ID3TagEditor.h"


	CID3Reader id3Reader;

// CID3TagEditor dialog

IMPLEMENT_DYNAMIC(CID3TagEditor, CDialog)

CID3TagEditor::CID3TagEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CID3TagEditor::IDD, pParent)
{

}

CID3TagEditor::~CID3TagEditor()
{
}

void CID3TagEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_filepath);
	DDX_Control(pDX, IDC_EDIT2, m_title);
	DDX_Control(pDX, IDC_EDIT3, m_artist);
	DDX_Control(pDX, IDC_EDIT4, m_album);
	DDX_Control(pDX, IDC_EDIT5, m_genre);
	DDX_Control(pDX, IDC_EDIT6, m_year);
	DDX_Control(pDX, IDC_EDIT7, m_comment);
	DDX_Control(pDX, IDC_BUTTON13, b_apicExport);
	DDX_Control(pDX, IDC_BUTTON14, b_apicImport);
	DDX_Control(pDX, IDC_BUTTON16, m_apply);
	DDX_Control(pDX, IDC_BUTTON15, m_picpreview);
}


BEGIN_MESSAGE_MAP(CID3TagEditor, CDialog)
//	ON_WM_CLOSE()
//ON_WM_NCDESTROY()
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BUTTON13, &CID3TagEditor::OnBnClickedButton13)
ON_BN_CLICKED(IDC_BUTTON14, &CID3TagEditor::OnBnClickedButton14)
ON_BN_CLICKED(IDC_BUTTON15, &CID3TagEditor::OnBnClickedButton15)
ON_BN_CLICKED(IDC_BUTTON16, &CID3TagEditor::OnBnClickedButton16)
ON_WM_PAINT()
ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CID3TagEditor message handlers

//void CID3TagEditor::OnClose()
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialog::OnClose();
//}

//void CID3TagEditor::OnNcDestroy()
//{
//	CDialog::OnNcDestroy();
//
//	// TODO: Add your message handler code here
//	DestroyWindow();
//	delete this;
//}

BOOL CID3TagEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	initCtrl();

	return FALSE;
}

void CID3TagEditor::initCtrl()
{
	// clear
	m_title.SetWindowTextW(L"");
	m_artist.SetWindowTextW(L"");
	m_album.SetWindowTextW(L"");
	m_genre.SetWindowTextW(L"");
	m_year.SetWindowTextW(L"");
	m_comment.SetWindowTextW(L"");
	b_apicExport.EnableWindow(FALSE);
	m_picpreview.EnableWindow(FALSE);
	b_apicImport.EnableWindow(FALSE);
	m_apply.EnableWindow(FALSE);
	lstrcpy(picPath, L"");
}

bool CID3TagEditor::loadFileID3Tag(CString filename)
{
	m_filepath.SetWindowTextW(filename);

	// clear
	initCtrl();

	if (lstrlen(id.fileName) == 0) {
		m_apply.EnableWindow(TRUE);
		return true;
	}

	if (id3Reader.readMP3File(filename.GetBuffer(0))) {
		WCHAR buf[256];
		if (id3Reader.getUnicodeData("TIT2", buf))	m_title.SetWindowTextW(buf);
		if (id3Reader.getUnicodeData("TPE1", buf))	m_artist.SetWindowTextW(buf);
		if (id3Reader.getUnicodeData("TALB", buf))	m_album.SetWindowTextW(buf);
		if (id3Reader.getUnicodeData("TCON", buf))	m_genre.SetWindowTextW(buf);	// 원래는  TRCK
		if (id3Reader.getUnicodeData("TYER", buf))	m_year.SetWindowTextW(buf);
		if (id3Reader.getUnicodeData("COMM", buf))	m_comment.SetWindowTextW(buf);
		if (id3Reader.isDataExists("APIC"))		{	b_apicExport.EnableWindow(TRUE); m_picpreview.EnableWindow(TRUE);	}
		b_apicImport.EnableWindow(TRUE);
		m_apply.EnableWindow(TRUE);

		// Value Refresh


		return true;
	} else {
		return false;
	}
}
void CID3TagEditor::OnClose()
{
	CDialog::OnClose();
}
/*
void CID3TagEditor::OnBnClickedButton1()
{
	// 일괄변경 [아티스트]
	m_artist.GetWindowTextW(_id3Edit_val, 256);
	strcpy(_id3Edit_frame, "TPE1");
	//AfxBeginThread(ChangeFrame, this->GetParent());
	ChangeFrame(this->GetParent());
}

void CID3TagEditor::OnBnClickedButton9()
{
	// 일괄변경 [앨범]
	m_artist.GetWindowTextW(_id3Edit_val, 256);
	strcpy(_id3Edit_frame, "TALB");
	//AfxBeginThread(ChangeFrame, this->GetParent());
	ChangeFrame(this->GetParent());
}

void CID3TagEditor::OnBnClickedButton10()
{
	// 일괄변경 [장르]
	m_artist.GetWindowTextW(_id3Edit_val, 256);
	strcpy(_id3Edit_frame, "TCON");
	//AfxBeginThread(ChangeFrame, this->GetParent());
	ChangeFrame(this->GetParent());
}

void CID3TagEditor::OnBnClickedButton11()
{
	// 일괄변경 [출시년도]
	m_artist.GetWindowTextW(_id3Edit_val, 256);
	strcpy(_id3Edit_frame, "TYER");
	//AfxBeginThread(ChangeFrame, this->GetParent());
	ChangeFrame(this->GetParent());
}

void CID3TagEditor::OnBnClickedButton12()
{
	// 일괄변경 [코멘트]
	m_artist.GetWindowTextW(_id3Edit_val, 256);
	strcpy(_id3Edit_frame, "COMM");
	//AfxBeginThread(ChangeFrame, this->GetParent());
	ChangeFrame(this->GetParent());
}*/

void CID3TagEditor::OnBnClickedButton13()
{
	WCHAR fname[256];
	m_filepath.GetWindowTextW(fname, 256);

	// 이미지 추출
	CFileDialog fd(FALSE, NULL, NULL, 4|2, L"JPEG Files|*.jpg|All Files|*.*|");
	//wcscpy(fd.m_ofn.lpstrInitialDir, fname);
	fd.m_ofn.lpstrInitialDir = fname;

	if (fd.DoModal() == IDOK) {
		WCHAR fName[256];
		wcscpy(fName, fd.GetPathName().GetBuffer());
		int wLen = wcslen(fName);
		fName[wLen] = '.';
		fName[wLen+1] = 'j';
		fName[wLen+2] = 'p';
		fName[wLen+3] = 'g';
		fName[wLen+4] = '\0';

		id3Reader.saveAPIC2File( fName );
	}
}

void CID3TagEditor::OnBnClickedButton14()
{
	// 이미지 삽입
	CFileDialog fd(TRUE, NULL, NULL, 4|2|OFN_FILEMUSTEXIST, L"JPEG Files|*.jpg;*.jpeg|All Files|*.*|");
	if (fd.DoModal() == IDOK) {
		lstrcpy(picPath, fd.GetPathName().GetBuffer());
		
		id3Reader.loadFile2APIC( fd.GetPathName().GetBuffer() );
		id3Reader.updateID3Data();
		id3Reader.updateMP3File();

		b_apicExport.EnableWindow(TRUE);
		m_picpreview.EnableWindow(TRUE);
	}
}

void CID3TagEditor::OnBnClickedButton15()
{
	// 이미지 추출 및 보여주기
	id3Reader.saveAPIC2File( L".\\albumthumb.jpg" );

	CImage cImg;
	cImg.Load(L".\\albumthumb.jpg");
	if (!cImg.IsNull()) {
		this->SetWindowPos(0, 0, 0, 640, 320, SWP_NOMOVE);
		cImg.Draw(::GetDC(this->m_hWnd), 320, 0, 320, 320);
	}
//	this->Invalidate();

	//::ShellExecute(NULL, L"open", L"%temp%\\albumthumb.jpg", NULL, NULL, SW_SHOWNORMAL);

/*
	// 일괄삽입 [이미지]
	CFileDialog fd(TRUE, NULL, NULL, 4|2|OFN_FILEMUSTEXIST, L"JPEG Files|*.jpg;*.jpeg|All Files|*.*|");
	if (fd.DoModal() == IDOK) {
		wcscpy(_id3Edit_val, fd.GetPathName().GetBuffer());
		strcpy(_id3Edit_frame, "APIC");
		//AfxBeginThread(ChangeFrame, this->GetParent());
		ChangeFrame(this->GetParent());
	}*/
}

void CID3TagEditor::OnBnClickedButton16()
{
	// 적용
	WCHAR buf[256];

	m_title.GetWindowTextW(id.s_title, 256);
	m_artist.GetWindowTextW(id.s_artist, 256);
	m_album.GetWindowTextW(id.s_album, 256);
	m_genre.GetWindowTextW(id.s_genre, 256);
	m_year.GetWindowTextW(id.s_year, 256);
	m_comment.GetWindowTextW(id.s_comment, 256);
	lstrcpy(id.albumPath, picPath);

	CMP3LyricsMasterv003Dlg *CParentDlg = (CMP3LyricsMasterv003Dlg *)this->GetParent();
	CParentDlg->editCurrentData(&id);
}

//void CID3TagEditor::OnEnChangeEdit2()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Add your control notification handler code here
//	m_title.SetSel(9999);
//}


void CID3TagEditor::editMode()
{
	// 부모 폼으로부터 선택된 아이템의 정보를 얻어온다
	CMP3LyricsMasterv003Dlg *CParentDlg = (CMP3LyricsMasterv003Dlg *)this->GetParent();
	CParentDlg->getCurrentData(&id);

	// reset frm size
	this->SetWindowPos(0, 0, 0, 320, 320, SWP_NOMOVE);

	loadFileID3Tag(id.fileName);
}
void CID3TagEditor::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// paint when image loaded
	if (!cImg.IsNull()) {
		ASSERT(cImg.Draw(dc, 320, 0, 320, 320));
	}
}


void CID3TagEditor::OnDropFiles(HDROP hDropInfo)
{
	UINT uDragCount = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	WCHAR szSourceFolder[MAX_PATH];

	if( uDragCount > 0 )
	{
		DragQueryFile( hDropInfo, 0, szSourceFolder, MAX_PATH );
		CString strPath = szSourceFolder;


		// 확장자 확인 후 앨범아트에 넣음
		if (strPath.Find(L".jpg") > 0) {
			lstrcpy(picPath, strPath.GetBuffer());
		
			id3Reader.loadFile2APIC( strPath.GetBuffer() );
			id3Reader.updateID3Data();
			id3Reader.updateMP3File();
			
			b_apicExport.EnableWindow(TRUE);
			m_picpreview.EnableWindow(TRUE);
		}
	}

	CDialog::OnDropFiles(hDropInfo);
}

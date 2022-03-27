// MP3 Lyrics Master v003Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MP3 Lyrics Master v003.h"
#include "MP3 Lyrics Master v003Dlg.h"

#include "TwitPicUpload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ��������ó��
CID3TagEditor cID3Dlg_main;
CID3Reader cID3r_main;
ITEMDATA itemArr[10000];


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMP3LyricsMasterv003Dlg ��ȭ ����




CMP3LyricsMasterv003Dlg::CMP3LyricsMasterv003Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMP3LyricsMasterv003Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	cID3Dlg = NULL;
}

void CMP3LyricsMasterv003Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, cb_flist);
	DDX_Control(pDX, IDC_CHECK2, cb_progress);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_BUTTON1, b_cancel);
	DDX_Control(pDX, IDC_BUTTON8, b_add);
	DDX_Control(pDX, IDC_LIST1, lb_flist);
	DDX_Control(pDX, IDC_LIST2, lb_progress);
	DDX_Control(pDX, IDC_BUTTON2, m_lyrics);
	DDX_Control(pDX, IDC_BUTTON3, m_lrcExport);
	DDX_Control(pDX, IDC_BUTTON4, m_lrcImport);
	DDX_Control(pDX, IDC_BUTTON5, m_pref);
	DDX_Control(pDX, IDC_BUTTON6, m_rename);
	DDX_Control(pDX, IDC_BUTTON7, m_editTag);
	DDX_Control(pDX, IDC_BUTTON17, b_delete);
}

BEGIN_MESSAGE_MAP(CMP3LyricsMasterv003Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON7, &CMP3LyricsMasterv003Dlg::OnBnClickedButton7)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON5, &CMP3LyricsMasterv003Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CMP3LyricsMasterv003Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CMP3LyricsMasterv003Dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMP3LyricsMasterv003Dlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMP3LyricsMasterv003Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMP3LyricsMasterv003Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMP3LyricsMasterv003Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CMP3LyricsMasterv003Dlg::OnBnClickedButton6)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMP3LyricsMasterv003Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON17, &CMP3LyricsMasterv003Dlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON8, &CMP3LyricsMasterv003Dlg::OnBnClickedButton8)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CMP3LyricsMasterv003Dlg::OnHdnItemclickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMP3LyricsMasterv003Dlg::OnLvnItemchangedList1)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMP3LyricsMasterv003Dlg::OnNMDblclkList1)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CMP3LyricsMasterv003Dlg::OnLvnKeydownList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMP3LyricsMasterv003Dlg::OnNMRClickList1)
	ON_COMMAND(ID_CM_TWITPIC, &CMP3LyricsMasterv003Dlg::OnCmTwitpic)
	ON_COMMAND(ID_CM_LYRICS, &CMP3LyricsMasterv003Dlg::OnCmLyrics)
	ON_COMMAND(ID_CM_FOLDER, &CMP3LyricsMasterv003Dlg::OnCmFolder)
	ON_COMMAND(ID_CM_REMOVE, &CMP3LyricsMasterv003Dlg::OnCmRemove)
	ON_COMMAND(ID_CM_LMDEL, &CMP3LyricsMasterv003Dlg::OnCmLmdel)
END_MESSAGE_MAP()


// CMP3LyricsMasterv003Dlg �޽��� ó����

int CMP3LyricsMasterv003Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �� ��ŷ
	/*
	cID3Dlg_main.Create(L"DockTest", this, TRUE, IDD_DIALOG1, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	cID3Dlg_main.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&cID3Dlg_main);
	*/

	return 0;
}

BOOL CMP3LyricsMasterv003Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// Add initalize
	LoadSetting();					// ���α׷� ������ �ɼ��� �о����
	toggleViewMode(TRUE, this);		// ���� ����Ʈ ����� �⺻���� ����
	toggleEnable(TRUE, this);		// ��Ʈ�ѵ��� �⺻������ ������ �����ϵ��� ����

	// form Resize
	loadFrmInfo();
	this->SetWindowPos(0, 0, 0, frm_width, frm_height, SWP_NOMOVE);

	// create Subform
	cID3Dlg_main.Create(IDD_DIALOG1, this);
	cID3Dlg_main.ShowWindow(SW_HIDE);
	
	// Group Set
	lb_flist.InsertColumn(0, L"�����̸�", LVCFMT_LEFT, 320);
	lb_flist.InsertColumn(1, L"����", LVCFMT_LEFT, 120);
	lb_flist.InsertColumn(2, L"��Ƽ��Ʈ", LVCFMT_LEFT, 120);
    lb_flist.InsertColumn(3, L"�ٹ�Ŀ��", LVCFMT_LEFT, 60);
    lb_flist.InsertColumn(4, L"�뷮", LVCFMT_LEFT, 60);
	
	lb_progress.InsertColumn(0, L"����", LVCFMT_LEFT, 40);
    lb_progress.InsertColumn(1, L"�����̸�", LVCFMT_LEFT, 320);
    lb_progress.InsertColumn(2, L"���λ���", LVCFMT_LEFT, 320);

	// �׷����
	ind_group = 0;
	ind_item=0;
	lb_flist.EnableGroupView(TRUE);

	DragAcceptFiles();

	return FALSE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMP3LyricsMasterv003Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMP3LyricsMasterv003Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMP3LyricsMasterv003Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMP3LyricsMasterv003Dlg::OnBnClickedButton7()
{
	// �����찡 ������ ������ ����
	if (!cID3Dlg_main.IsWindowVisible() && lb_flist.GetSelectedCount() > 0) {
		cID3Dlg_main.editMode();
		cID3Dlg_main.ShowWindow(SW_SHOW);
	}
}

void CMP3LyricsMasterv003Dlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	if (lpwndpos->cx < 420 || lpwndpos->cy < 300) return;
	CDialog::OnWindowPosChanged(lpwndpos);

	// TODO: Add your message handler code here
	
	/* ���̾ƿ� �缳�� */
	CRect _tr;
	_tr.left = lpwndpos->x;
	_tr.top = lpwndpos->y;
	_tr.right = _tr.left + lpwndpos->cx;
	_tr.bottom = _tr.top + lpwndpos->cy;
	resetLayout(&_tr);

	/* ����â */
	if (cID3Dlg_main.m_hWnd != NULL) {
		cID3Dlg_main.SetWindowPos(NULL, lpwndpos->x+lpwndpos->cx, lpwndpos->y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	frm_width = _tr.Width();
	frm_height = _tr.Height();
}

void CMP3LyricsMasterv003Dlg::OnDropFiles(HDROP hDropInfo)
{
	UINT uDragCount = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	WCHAR szSourceFolder[MAX_PATH];

	if( uDragCount > 0 )
	{
		for (UINT i=0; i<uDragCount; i++)
		{
			DragQueryFile( hDropInfo, i, szSourceFolder, MAX_PATH );
			CString strPath = szSourceFolder;
			SearchDirectory(strPath);
		}
	}

	CDialog::OnDropFiles(hDropInfo);
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton5()
{
	// show Preferences Window
	CPreference cpDlg;
	cpDlg.DoModal();
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton2()
{
	// do Lyrics Marking
	insertSelItem2Progress();
	AfxBeginThread(MarkMP3Lyrics, this);
}

void CMP3LyricsMasterv003Dlg::OnBnClickedCheck1()
{
	toggleViewMode(TRUE, this);
}

void CMP3LyricsMasterv003Dlg::OnBnClickedCheck2()
{
	toggleViewMode(FALSE, this);
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton1()
{
	// �۾� �ߴ�
	doProcess = false;
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton3()
{
	// extract LRC File
	insertSelItem2Progress();
	AfxBeginThread(ExtractMP3Lyrics, this);
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton4()
{
	// input LRC File
	insertSelItem2Progress();
	AfxBeginThread(ImportMP3Lyrics, this);
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton6()
{
	// find APICs
	insertSelItem2Progress();
	AfxBeginThread(ImportAPIC, this);
}

void CMP3LyricsMasterv003Dlg::OnLbnSelchangeList1()
{
	// send message to Window
	/*if (lb_flist.GetCurSel() >= 0 && cID3Dlg_main) {
		CString buf;
		lb_flist.GetText(lb_flist.GetCurSel(), buf);
		cID3Dlg_main.loadFileID3Tag(buf);
	}*/
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton17()
{
	// delete list
	if (cb_flist.GetCheck()) {
		//clearFList(this);
		while (lb_flist.GetSelectedCount()>0) {
			lb_flist.DeleteItem(lb_flist.GetNextItem(-1, LVNI_SELECTED));
		}
	} else {
		//clearPList(this);
	}
}

void CMP3LyricsMasterv003Dlg::OnBnClickedButton8()
{
	// ���� �߰�
	CFileDialog fd(TRUE, NULL, NULL, 4|2|OFN_ALLOWMULTISELECT, L"MP3 Files|*.mp3|All Files|*.*|");
	if (fd.DoModal() == IDOK) {
		POSITION startPosition;
		startPosition = fd.GetStartPosition();
		while (startPosition) {
			CString fPath = fd.GetNextPathName(startPosition);
			insertItem(fPath.GetBuffer());
		}
	}
}

int CMP3LyricsMasterv003Dlg::checkIsGroupExist(CListCtrl &clc, WCHAR *str)
{
	for (int i=0; i<ind_group; i++)
	{
		LVGROUP lg;
		::ZeroMemory(&lg, sizeof(LVGROUP));
		lg.cbSize = sizeof(LVGROUP);
		lg.mask = LVGF_GROUPID | LVGF_HEADER | LVGF_STATE | LVGF_NONE; // �� : LVGF_ALIGN - �ɼ� �����

		if (clc.GetGroupInfo(i, &lg) != -1) {
			CString sh = lg.pszHeader;
			if (memcmp(sh.GetBuffer(), str, (wcslen(str)-1)*2) == 0) return i;
		}
	}

	return -1;
}
int CMP3LyricsMasterv003Dlg::insertGroup(CListCtrl &clc, WCHAR *str)
{
	// �׷��� �������� ���� �� �׷��� �ϳ� �߰��Ѵ�
	int gi = checkIsGroupExist(clc, str);

	if (gi < 0) {
		LVGROUP group;
		::ZeroMemory(&group, sizeof(group));
		group.cbSize = sizeof(group);   
		group.uAlign = LVGA_HEADER_LEFT;
		group.mask = LVGF_HEADER|LVGF_GROUPID|LVGF_ALIGN;
		group.pszHeader = str;
		group.iGroupId = ind_group;
		lb_flist.InsertGroup(-1, &group);

		ind_group++;
		return ind_group-1;
	}

	return gi;
}
bool CMP3LyricsMasterv003Dlg::isDuplicate(WCHAR *filename)
{
	for (int i=0; i<lb_flist.GetItemCount(); i++) {
		CString str = lb_flist.GetItemText(i, 0);
		if (wcscmp(str.GetBuffer(), filename) == 0) return true;
	}

	return false;
}
void CMP3LyricsMasterv003Dlg::insertItem(WCHAR *filename)
{
	WCHAR ext[10];
	lstrcpy(ext, filename+lstrlen(filename)-4);
	if (lstrcmp(CString(ext).MakeUpper().GetBuffer(), L".MP3")!=0) return;
	if (isDuplicate(filename)) return;

	ITEMDATA id;

	// Filename ä���
	lstrcpy(id.fileName, filename);

	// FolderPath�� ����
	for (int i=wcslen(filename); i>0; i--) {
		if (filename[i] == L'\\') {
			wcscpy(id.folderPath, filename);
			id.folderPath[i+1] = L'\0';
			break;
		}
	}

	// SyncState�� ����
	id.albumCover = 0;
	lstrcpy(id.lyricsStr, L"���� ����");
	lstrcpy(id.s_title, L"");
	lstrcpy(id.s_artist, L"");
	if (cID3r_main.readMP3File(filename)) {
		////
		BYTE syncBuf[100000];
		int syncBufSize;
		if (cID3r_main.getRawData("USLT", syncBuf, &syncBufSize)) {
			if (syncBufSize > 6) {
				// get str
				if (memcmp(syncBuf+6, "[M_CITY]", 8)==0) {
					wcscpy(id.lyricsStr, L"�Ｚ");
					id.lyricsState = M_SAMSUNG;
				} else if (memcmp(syncBuf+6, "[3 LINES]", 8)==0) {
					wcscpy(id.lyricsStr, L"���̸���");
					id.lyricsState = M_IRIVER;
				} else if (memcmp(syncBuf+6, "[iAUDIO ", 7)==0) {
					CString _buf;
					_buf.Format(L"�ڿ� %s", CString(syncBuf+6));
					wcscpy(id.lyricsStr, _buf);
					id.lyricsState = M_COWON;
				} else if (memcmp(syncBuf+6, "[COWON ", 6)==0) {
					CString _buf;
					_buf.Format(L"�ڿ� %s", CString(syncBuf+6));
					wcscpy(id.lyricsStr, _buf);
					id.lyricsState = M_COWON;
				} else if (syncBuf[0] == 0x01) {
					wcscpy(id.lyricsStr, L"����ƪ��");
					id.lyricsState = M_ITUNES;
				}
			}
		}

		WCHAR sBuf[256];
		if (cID3r_main.getUnicodeData("TIT2", sBuf)) lstrcpy(id.s_title, sBuf);
		if (cID3r_main.getUnicodeData("TPE1", sBuf)) lstrcpy(id.s_artist, sBuf);
		if (cID3r_main.getUnicodeData("TALB", sBuf)) lstrcpy(id.s_album, sBuf);
		if (cID3r_main.getUnicodeData("TCON", sBuf)) lstrcpy(id.s_genre, sBuf);
		if (cID3r_main.getUnicodeData("TYER", sBuf)) lstrcpy(id.s_year, sBuf);
		if (cID3r_main.getUnicodeData("COMM", sBuf)) lstrcpy(id.s_comment, sBuf);

		// AlbumCover ����
		if (cID3r_main.isDataExists("APIC")) {
			id.albumCover = 1;
		}
	}

	// ���� �뷮
	FILE *fp = _tfopen(filename, L"rb");
	if (fp == 0) id.fileSize = 0;
	else {
		fseek(fp, 0, SEEK_END);
		id.fileSize = ftell(fp);
		fclose(fp);
	}

	int i = insertGroup(lb_flist, id.lyricsStr);
	
	LVITEM lvi;
	::ZeroMemory(&lvi, sizeof(lvi));

	lvi.mask =  LVIF_TEXT|LVIF_GROUPID;
	id.index = lvi.iItem = ind_item;
	lvi.iSubItem = 0;
	lvi.iGroupId = i;
	lvi.pszText = filename;

	int l = lb_flist.InsertItem(&lvi);
	if (id.albumCover) lb_flist.SetItemText(l, 3, L"����"); else lb_flist.SetItemText(l, 3, L"����");
	CString _buf;
	_buf.Format(L"%.2fMB", (float)id.fileSize/1024/1024);
	lb_flist.SetItemText(l, 4, _buf.GetBuffer());
	memcpy(&itemArr[ind_item], &id, sizeof(ITEMDATA));
	lb_flist.SetItemText(l, 1, id.s_title);
	lb_flist.SetItemText(l, 2, id.s_artist);
	lb_flist.SetItemData(l, (LPARAM)&itemArr[ind_item]);

	ind_item++;
}

void CMP3LyricsMasterv003Dlg::SearchDirectory(CString parm_search_path)
{
    CString search_path = parm_search_path;
     
    WIN32_FIND_DATA file_data;
    // ���� ����� ��� ���ϰ� ���丮�� ã�´�.
    HANDLE search_handle = FindFirstFile(search_path + L"\\*.*", &file_data);
    if(INVALID_HANDLE_VALUE != search_handle) {
        do{
            if(FILE_ATTRIBUTE_DIRECTORY & file_data.dwFileAttributes){
                // ������ ���...
                // ���� ����(.)�� ��������("..")�� �ǹ��ϴ� �ɺ��� ���ܽ�Ų��.
                if(wcscmp(file_data.cFileName, L".") && wcscmp(file_data.cFileName, L"..")){
                    // ���� ���丮�� ��� �˻��Ѵ�.
                    SearchDirectory(search_path + CString("\\") + file_data.cFileName);
                }
            } else {
                // ������ ���..
				CString _buf = search_path + CString("\\") + file_data.cFileName;
				insertItem(_buf.GetBuffer());
            }
        } while(FindNextFile(search_handle, &file_data));
        FindClose(search_handle);
	} else {
		// �׳� �̰��� �ܼ� �����̾���
		insertItem(parm_search_path.GetBuffer());
	}
}

/* sort part */
int CALLBACK stringSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int r=0;

	//CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	ITEMDATA *id1 = (ITEMDATA*)lParam1;
	ITEMDATA *id2 = (ITEMDATA*)lParam2;

	switch (lParamSort) {
		case 0:	// �̸�
			r = lstrcmp(id1->fileName, id2->fileName);
			break;
		case 1:	// ����
			r = lstrcmp(id1->s_title, id2->s_title);
			break;
		case 2:	// ��Ƽ��Ʈ
			r = lstrcmp(id1->s_artist, id2->s_artist);
			break;
		case 3:	// �ٹ�Ŀ��
			r = (id1->albumCover < id2->albumCover);
			break;
		case 4:	// ���縶ŷ
			r = (id1->lyricsState < id2->lyricsState);
			break;
		case 5:	// �뷮
			r = (id1->fileSize < id2->fileSize);
			break;
		default:
			break;
	}

	return r;
}
/* sort part end */

void CMP3LyricsMasterv003Dlg::OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	lb_flist.SortItems(stringSort, phdr->iItem);

	*pResult = 0;
}
bool CMP3LyricsMasterv003Dlg::getCurrentData(ITEMDATA *id)
{
	// �������϶��� true�̵� ZeroMemory
	if (lb_flist.GetSelectedCount() > 0) {
		if (lb_flist.GetSelectedCount() == 1) {
			memcpy(id, (LPVOID)lb_flist.GetItemData(lb_flist.GetNextItem(-1, LVNI_SELECTED)), sizeof(ITEMDATA));
		} else {
			::ZeroMemory(id, sizeof(ITEMDATA));
		}
		return true;
	} else {
		return false;
	}
}
bool CMP3LyricsMasterv003Dlg::editCurrentData(ITEMDATA *id)
{
	// �������϶��� filename �����ϰ� �����Ű��
	// �ٹ�Ŀ�������� ��� �ذ�����? > ���� �������� ������, ������ �״��

	if (lb_flist.GetSelectedCount() > 0) {
		int i = lb_flist.GetNextItem(-1, LVNI_SELECTED);
		if (lb_flist.GetSelectedCount() == 1) {
				ITEMDATA *tid;
				tid = (ITEMDATA *)lb_flist.GetItemData(i);
				if (cID3r_main.readMP3File(tid->fileName)) {
					memcpy(tid, id, sizeof(ITEMDATA));

					cID3r_main.setUnicodeData("TIT2", id->s_title, lstrlen(id->s_title));
					cID3r_main.setUnicodeData("TPE1", id->s_artist, lstrlen(id->s_artist));
					cID3r_main.setUnicodeData("TALB", id->s_album, lstrlen(id->s_album));
					cID3r_main.setUnicodeData("TCON", id->s_genre, lstrlen(id->s_genre));
					cID3r_main.setUnicodeData("TYER", id->s_year, lstrlen(id->s_year));
					cID3r_main.setUnicodeData("COMM", id->s_comment, lstrlen(id->s_comment));
					if (lstrlen(id->albumPath) > 0) {
						cID3r_main.loadFile2APIC(id->albumPath);
						tid->albumCover = 1;
					}
					
					lb_flist.SetItemText(i, 1, id->s_title);
					lb_flist.SetItemText(i, 2, id->s_artist);
					if (tid->albumCover) lb_flist.SetItemText(i, 3, L"����"); else lb_flist.SetItemText(i, 3, L"����");
					
					cID3r_main.updateID3Data();
					cID3r_main.updateMP3File();
				} else {
					addMessage(L"����", _T("���� ���� ���� : %s", tid->fileName), this);
				}
				memcpy(tid, id, sizeof(ITEMDATA));
		} else {
			while (i>=0) {
				ITEMDATA *tid;
				tid = (ITEMDATA *)lb_flist.GetItemData(i);
				if (cID3r_main.readMP3File(tid->fileName)) {
					/*DATA PROCESS*/
					if (lstrlen(id->s_title) > 0)
						cID3r_main.setUnicodeData("TIT2", id->s_title, lstrlen(id->s_title)), lstrcpy(tid->s_title, id->s_title);
					if (lstrlen(id->s_artist) > 0)
						cID3r_main.setUnicodeData("TPE1", id->s_artist, lstrlen(id->s_artist)), lstrcpy(tid->s_artist, id->s_artist);
					if (lstrlen(id->s_album) > 0)
						cID3r_main.setUnicodeData("TALB", id->s_album, lstrlen(id->s_album)), lstrcpy(tid->s_album, id->s_album);
					if (lstrlen(id->s_genre) > 0)
						cID3r_main.setUnicodeData("TCON", id->s_genre, lstrlen(id->s_genre)), lstrcpy(tid->s_genre, id->s_genre);
					if (lstrlen(id->s_year) > 0)
						cID3r_main.setUnicodeData("TYER", id->s_year, lstrlen(id->s_year)), lstrcpy(tid->s_year, id->s_year);
					if (lstrlen(id->s_comment) > 0)
						cID3r_main.setUnicodeData("COMM", id->s_comment, lstrlen(id->s_comment)), lstrcpy(tid->s_comment, id->s_comment);
					if (lstrlen(id->albumPath) > 0) {
						cID3r_main.loadFile2APIC(id->albumPath);
						tid->albumCover = 1;
					}
					
					lb_flist.SetItemText(i, 1, tid->s_title);
					lb_flist.SetItemText(i, 2, tid->s_artist);
					if (tid->albumCover) lb_flist.SetItemText(i, 3, L"����"); else lb_flist.SetItemText(i, 3, L"����");
					
					cID3r_main.updateID3Data();
					cID3r_main.updateMP3File();
					/*DATA PROCESS END*/
				} else {
					addMessage(L"����", _T("���� ���� ���� : %s", tid->fileName), this);
				}
				i = lb_flist.GetNextItem(i, LVNI_SELECTED);
			}
		}
		return true;
	} else {
		return false;
	}
}
void CMP3LyricsMasterv003Dlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// close Window if opened
	if (cID3Dlg_main.IsWindowVisible()) {
		cID3Dlg_main.ShowWindow(SW_HIDE);
	}

	*pResult = 0;
}

void CMP3LyricsMasterv003Dlg::insertSelItem2Progress()
{
	lb_progress.DeleteAllItems();
	
	LVITEM lvi;
	::ZeroMemory(&lvi, sizeof(lvi));
	lvi.mask =  LVIF_TEXT;
	lvi.iItem = ind_item;

	int ni = lb_flist.GetNextItem(-1, LVNI_SELECTED);
	while (ni != -1) {
		lvi.iItem = lb_progress.GetItemCount();
		int l = lb_progress.InsertItem(&lvi);
		lb_progress.SetItemData(l, lb_flist.GetItemData(ni));
		CString _t = lb_flist.GetItemText(ni, 0);
		lb_progress.SetItemText(l, 1, _t.GetBuffer());
		ni = lb_flist.GetNextItem(ni, LVNI_SELECTED);
	}
}

void CMP3LyricsMasterv003Dlg::resetLayout(CRect* wp)
{
	m_progress.SetWindowPos	(0, 0, 0, wp->Width()-280, 16, SWP_NOACTIVATE | SWP_NOMOVE);
	b_cancel.SetWindowPos	(0, wp->Width()-130, 12, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	b_add.SetWindowPos		(0, wp->Width()-95, 12, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	b_delete.SetWindowPos	(0, wp->Width()-60, 12, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	
	lb_flist.SetWindowPos	(0, 0, 0, wp->Width()-36, wp->Height()-130, SWP_NOACTIVATE | SWP_NOMOVE);
	lb_progress.SetWindowPos(0, 0, 0, wp->Width()-36, wp->Height()-130, SWP_NOACTIVATE | SWP_NOMOVE);

	float bWid = (wp->Width() - 5*5 - 20*2)/6.0f;
	m_lyrics.SetWindowPos	(0, 10, wp->Height()-80, bWid, 32, SWP_NOACTIVATE);
	m_lrcExport.SetWindowPos(0, 15+bWid, wp->Height()-80, bWid, 32, SWP_NOACTIVATE);
	m_lrcImport.SetWindowPos(0, 20+bWid*2, wp->Height()-80, bWid, 32, SWP_NOACTIVATE);
	m_pref.SetWindowPos		(0, 25+bWid*3, wp->Height()-80, bWid, 32, SWP_NOACTIVATE);
	m_rename.SetWindowPos	(0, 30+bWid*4, wp->Height()-80, bWid, 32, SWP_NOACTIVATE);
	m_editTag.SetWindowPos	(0, 35+bWid*5, wp->Height()-80, bWid, 32, SWP_NOACTIVATE);
}
void CMP3LyricsMasterv003Dlg::loadFrmInfo()
{
	char buf[255];
	GetPrivateProfileStringA( "PROG", "frm_width", "600", buf, 255, ".\\config.ini");			frm_width = atoi(buf);
	GetPrivateProfileStringA( "PROG", "frm_height", "320", buf, 255, ".\\config.ini");			frm_height = atoi(buf);
}
void CMP3LyricsMasterv003Dlg::saveFrmInfo()
{
	char buf[255];
	itoa(frm_width, buf, 10);			WritePrivateProfileStringA( "PROG", "frm_width", buf, ".\\config.ini" );
	itoa(frm_height, buf, 10);			WritePrivateProfileStringA( "PROG", "frm_height", buf, ".\\config.ini" );
}
void CMP3LyricsMasterv003Dlg::OnClose()
{
	saveFrmInfo();

	CDialog::OnClose();
}

void CMP3LyricsMasterv003Dlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// ���� ���� â ����
	OnCmLyrics();

	*pResult = 0;
}

void CMP3LyricsMasterv003Dlg::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	switch(pLVKeyDow->wVKey)
	{
	case 'T':
		OnCmTwitpic();
		break;
	}

	*pResult = 0;
}


void CMP3LyricsMasterv003Dlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// �޴��� ����
	
	int ni = lb_flist.GetNextItem(-1, LVNI_SELECTED);
	if (ni < 0) return;

	POINT p;
	::GetCursorPos(&p);

	CMenu menu;
	menu.LoadMenuW( IDR_MENU1);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_HORPOSANIMATION, p.x, p.y, this);

	*pResult = 0;
}


void CMP3LyricsMasterv003Dlg::OnCmTwitpic()
{
	int ni = lb_flist.GetNextItem(-1, LVNI_SELECTED);
	if (ni < 0) return;
	ITEMDATA *tid = (ITEMDATA*)lb_flist.GetItemData(ni);
	if (!tid->albumCover)
	{
		AfxMessageBox(L"�ٹ� Ŀ���� �����ϴ�!");
		return ;
	}

	CString sMsg, sAlbum, sTitle, sArtist;
	sAlbum = tid->s_album;
	sTitle = tid->s_title;
	sArtist = tid->s_artist;

	if (sTitle == "")
	{
		CString _ts = tid->fileName;
		int i = _ts.ReverseFind(L'\\');
		sTitle = _ts.Mid(i, _ts.GetLength()-i-3);
		AfxMessageBox(sTitle);
	}

	sMsg.Format(L"#music %s", sTitle);
	if (sArtist != "") sMsg += L" - " + sArtist;
	if (sAlbum != "") sMsg += L" - [" + sAlbum + L"]";

	CTwitPicUpload tpUploadDlg;
	tpUploadDlg.picText = sMsg;
	if (!cID3r_main.readMP3File(tid->fileName))
		AfxMessageBox(L"Failed to Access MP3 File");
	cID3r_main.saveAPIC2File(L".\\album.jpg");
	tpUploadDlg.picPath = L".\\album.jpg";
	tpUploadDlg.DoModal();
}


void CMP3LyricsMasterv003Dlg::OnCmLyrics()
{
	int ni = lb_flist.GetNextItem(-1, LVNI_SELECTED);
	if (ni < 0) return;
	LVDlg ldlg;
	ldlg.fPath = lb_flist.GetItemText(ni, 0);
	ITEMDATA *id;
	id = (ITEMDATA*)lb_flist.GetItemData(ni);
	ldlg.sModel = id->lyricsStr;
	ldlg.iModel = id->lyricsState;
	ldlg.DoModal();
}


void CMP3LyricsMasterv003Dlg::OnCmFolder()
{
	int ni = lb_flist.GetNextItem(-1, LVNI_SELECTED);
	if (ni < 0) return;
	ITEMDATA *tid = (ITEMDATA*)lb_flist.GetItemData(ni);

	CString cmd;
	cmd.Format(L"%s \"%s\"", L"explorer.exe", tid->folderPath);

	::WinExec(CStringA(cmd), SW_SHOWNORMAL);
}


void CMP3LyricsMasterv003Dlg::OnCmRemove()
{
	if (MessageBox(L"������ ���� ���� �Ͻðڽ��ϱ�?", L"Ȯ��", MB_YESNO) == IDNO) return;

	int ni;

	if (cb_flist.GetCheck()) {
		while (lb_flist.GetSelectedCount()>0) {
			ni = lb_flist.GetNextItem(-1, LVNI_SELECTED);
			ITEMDATA *tid = (ITEMDATA*)lb_flist.GetItemData(ni);
			_wremove(tid->fileName);
			lb_flist.DeleteItem(ni);
		}
	}
}


void CMP3LyricsMasterv003Dlg::OnCmLmdel()
{
	int ni;

	if (cb_flist.GetCheck()) {
		while (lb_flist.GetSelectedCount()>0) {
			ni = lb_flist.GetNextItem(-1, LVNI_SELECTED);
			lb_flist.DeleteItem(ni);
		}
	}
}

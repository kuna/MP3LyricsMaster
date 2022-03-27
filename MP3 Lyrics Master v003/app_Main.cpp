#include "stdafx.h"
#include "app_Main.h"

#include "id3.h"
#include "lyrics.h"
#include "APICSearcher.h"
#include "uslt_samsung.h"
#include "uslt_iriver.h"
#include "uslt_cowon.h"
#include "uslt_itune.h"
#include "MD5Maker.h"		// create hash

#include "resource.h"
#include "MP3 Lyrics Master v003Dlg.h"

// custom
int b_autoWrap;


void toggleViewMode(BOOL isFileList, LPVOID lpWnd)
{
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lpWnd;
	if (isFileList) {
		cDlg->cb_flist.SetCheck(TRUE);
		cDlg->cb_progress.SetCheck(FALSE);
		cDlg->lb_flist.ShowWindow(SW_SHOW);
		cDlg->lb_progress.ShowWindow(SW_HIDE);
	} else {
		cDlg->cb_flist.SetCheck(FALSE);
		cDlg->cb_progress.SetCheck(TRUE);
		cDlg->lb_flist.ShowWindow(SW_HIDE);
		cDlg->lb_progress.ShowWindow(SW_SHOW);
	}
}

void toggleEnable(BOOL isEnabled, LPVOID lpWnd)
{
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lpWnd;
	if (isEnabled) {
		cDlg->cb_flist.EnableWindow();
		cDlg->cb_progress.EnableWindow();
		cDlg->b_add.EnableWindow();
		cDlg->m_editTag.EnableWindow();
		cDlg->m_lrcExport.EnableWindow();
		cDlg->m_lrcImport.EnableWindow();
		cDlg->m_lyrics.EnableWindow();
		cDlg->m_pref.EnableWindow();
		cDlg->m_progress.EnableWindow();
		cDlg->m_rename.EnableWindow();

		cDlg->b_cancel.EnableWindow(FALSE);
	} else {
		cDlg->cb_flist.EnableWindow(FALSE);
		cDlg->cb_progress.EnableWindow(FALSE);
		cDlg->b_add.EnableWindow(FALSE);
		cDlg->m_editTag.EnableWindow(FALSE);
		cDlg->m_lrcExport.EnableWindow(FALSE);
		cDlg->m_lrcImport.EnableWindow(FALSE);
		cDlg->m_lyrics.EnableWindow(FALSE);
		cDlg->m_pref.EnableWindow(FALSE);
		cDlg->m_progress.EnableWindow(FALSE);
		cDlg->m_rename.EnableWindow(FALSE);

		cDlg->b_cancel.EnableWindow();

		// Close ID3 Edit Window ... I DONT KNOW WHY IT LOSES POINTER!
		/*if (cDlg->cID3Dlg != NULL) {
			BOOL res = (cDlg->cID3Dlg)->DestroyWindow();
			delete cDlg->cID3Dlg;
			cDlg->cID3Dlg = NULL;
		}*/
	}
}

void clearFList(LPVOID lpWnd)
{
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lpWnd;
	cDlg->lb_flist.DeleteAllItems();
}
void clearPList(LPVOID lpWnd)
{
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lpWnd;
	cDlg->lb_progress.DeleteAllItems();
}

void addMessage(CString stat, CString msg, LPVOID lpWnd)
{
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lpWnd;
	int i = cDlg->lb_progress.GetItemCount();

	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
// Insert the first item
	lvi.mask =  LVIF_TEXT;
	lvi.iItem = i;
	lvi.iSubItem = 0;
	lvi.pszText = stat.GetBuffer();
	cDlg->lb_progress.InsertItem(&lvi);
// Set subitem 1
	lvi.iSubItem = 1;
	lvi.pszText = msg.GetBuffer();
	cDlg->lb_progress.SetItem(&lvi);
}

void SaveSetting()
{
	char buf[255];
	itoa(ls1, buf, 10);			WritePrivateProfileStringA( "SETTING", "ls1", buf, ".\\config.ini" );
	itoa(ls2, buf, 10);			WritePrivateProfileStringA( "SETTING", "ls2", buf, ".\\config.ini" );
	itoa(ls3, buf, 10);			WritePrivateProfileStringA( "SETTING", "ls3", buf, ".\\config.ini" );
	itoa(ls4, buf, 10);			WritePrivateProfileStringA( "SETTING", "ls4", buf, ".\\config.ini" );
	itoa(mp3_Machine, buf, 10);	WritePrivateProfileStringA( "SETTING", "mp3_Machine", buf, ".\\config.ini" );
	itoa(ls_alsong, buf, 10);	WritePrivateProfileStringA( "SETTING", "ls_alsong", buf, ".\\config.ini" );
	itoa(ls_gomaudio, buf, 10);	WritePrivateProfileStringA( "SETTING", "ls_gomaudio", buf, ".\\config.ini" );
	
	itoa(b_remark, buf, 10);		WritePrivateProfileStringA( "SETTING", "remark", buf, ".\\config.ini" );
	itoa(b_tagautofix, buf, 10);	WritePrivateProfileStringA( "SETTING", "tagautofix", buf, ".\\config.ini" );
	itoa(b_tagfind, buf, 10);		WritePrivateProfileStringA( "SETTING", "tagfind", buf, ".\\config.ini" );

	itoa(lrcUnicode, buf, 10);	WritePrivateProfileStringA( "PROG", "lrcUnicode", buf, ".\\config.ini");
	WritePrivateProfileStringA( "DEVICE", "cowonDevice", cowonDevice, ".\\config.ini" );
}

void LoadSetting()
{
	char buf[255];
	GetPrivateProfileStringA( "SETTING", "ls1", "1", buf, 255, ".\\config.ini");			ls1 = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "ls2", "1", buf, 255, ".\\config.ini");			ls2 = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "ls3", "1", buf, 255, ".\\config.ini");			ls3 = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "ls4", "1", buf, 255, ".\\config.ini");			ls4 = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "mp3_Machine", "1", buf, 255, ".\\config.ini");	mp3_Machine = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "ls_alsong", "1", buf, 255, ".\\config.ini");		ls_alsong = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "ls_gomaudio", "1", buf, 255, ".\\config.ini");	ls_gomaudio = atoi(buf);
	
	GetPrivateProfileStringA( "SETTING", "remark", "0", buf, 255, ".\\config.ini");			b_remark = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "tagautofix", "0", buf, 255, ".\\config.ini");		b_tagautofix = atoi(buf);
	GetPrivateProfileStringA( "SETTING", "tagfind", "0", buf, 255, ".\\config.ini");		b_tagfind = atoi(buf);
	
	GetPrivateProfileStringA( "PROG", "s_recvSize", "10000", buf, 255, ".\\config.ini");	s_recvSize = atoi(buf);
	GetPrivateProfileStringA( "PROG", "s_recvDelay", "0", buf, 255, ".\\config.ini");	s_recvDelay = atoi(buf);
	GetPrivateProfileStringA( "PROG", "lrcUnicode", "1", buf, 255, ".\\config.ini");	lrcUnicode = atoi(buf);

	GetPrivateProfileStringA( "DEVICE", "cowonDevice", "", cowonDevice, 255, ".\\config.ini");


	//custom
	GetPrivateProfileStringA( "SETTING", "b_autoWrap", "1", buf, 255, ".\\config.ini");	b_autoWrap = atoi(buf);
}

UINT MarkMP3Lyrics(LPVOID lParam)
{
	if (doProcess) return 0;
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lParam;
	CID3Reader *id3Reader = new CID3Reader();

	// starting Job
	toggleEnable(FALSE, lParam);
	toggleViewMode(FALSE, lParam);

	// progress
	WCHAR filepath[256];
	int nCnt = cDlg->lb_progress.GetItemCount();

	doProcess = true;
	for (int i=0; i<nCnt; i++)
	{
		/* cancel part */
		if (!doProcess) {
			toggleEnable(TRUE, lParam);
			return 0;
		}
		/* cancel part end */

		cDlg->lb_progress.SetItemText(i, 0, L"->");

		ITEMDATA *id;
		id = (ITEMDATA*)cDlg->lb_progress.GetItemData(i);
		wcscpy(filepath, id->fileName);

		if (!id3Reader->readMP3File(filepath))
		{
			cDlg->lb_progress.SetItemText(i, 0, L"에러");
			cDlg->lb_progress.SetItemText(i, 2, L"올바른 mp3 파일이 아닙니다");
		} else {
			// work
			CLyrics cLrc;

			// get Lyrics
			bool res=false;
			CMD5Maker maker;
			int len, offset = id3Reader->mp3DataStartPos;
			WCHAR MD5Hash[255];
			if (!res && ls_alsong) {
				len = H_ALSONG;
				maker.makeMD5AdvanceW(filepath, MD5Hash, offset, len);
				res = cLrc.getLRCFromAlsongServer(MD5Hash);
				if (!res && b_tagfind)
					res = cLrc.getLRCFromAlsongServerTA(id->s_title, id->s_artist);
			}
			if (!res && ls_gomaudio) {
				len = H_GOMAUDIO;
				maker.makeMD5AdvanceW(filepath, MD5Hash, offset, len);
				res = cLrc.getLRCFromGOMServer(MD5Hash, L"", L"", L"");
			}

			if (!res) {
				cDlg->lb_progress.SetItemText(i, 0, L"실패");
				cDlg->lb_progress.SetItemText(i, 2, L"싱크 가사가 없습니다");
			} else if (b_remark && id->lyricsState == mp3_Machine) {
				cDlg->lb_progress.SetItemText(i, 0, L"무시");
				cDlg->lb_progress.SetItemText(i, 2, L"이미 동일 기종의 싱크 가사가 존재합니다");
			} else {
				if (mp3_Machine == M_ITUNES)
					cLrc.processLyrics(ls1, ls2, ls3, ls4, false, b_autoWrap);
				else
					cLrc.processLyrics(ls1, ls2, ls3, ls4, true, b_autoWrap);

				BYTE *uData = (BYTE *)malloc(sizeof(BYTE) * LARGEBUF);
				int uLen;
				WCHAR nGroupName[256];

				if (mp3_Machine == M_SAMSUNG) {
					CUSLTSamsung sUSLT;
					sUSLT.Lyrics2SamsungUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_time, cLrc.l_cnt);
					lstrcpy(nGroupName, L"삼성");
				} else if (mp3_Machine == M_IRIVER) {
					CUSLTIriver iUSLT;
					iUSLT.Lyrics2IriverUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_time, cLrc.l_cnt);
					lstrcpy(nGroupName, L"아이리버");
				} else if (mp3_Machine == M_COWON) {
					CUSLTCowon cUSLT;
					strcpy(cUSLT.cowonDevice, cowonDevice);
					cUSLT.Lyrics2CowonUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_time, cLrc.l_cnt);
					CString _buf;
					_buf.Format(L"코원 [%s]", CString(cowonDevice));
					lstrcpy(nGroupName, _buf.GetBuffer());
				} else if (mp3_Machine == M_ITUNES) {
					CUSLTITunes iUSLT;
					iUSLT.Lyrics2ITunesUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_cnt);
					lstrcpy(nGroupName, L"아이튠스");
				}

				// AUTO FIX PART
				if (b_tagautofix) {
					id3Reader->setUnicodeData("TIT2", cLrc.l_ti, lstrlen(cLrc.l_ti));
					id3Reader->setUnicodeData("TPE1", cLrc.l_ar, lstrlen(cLrc.l_ar));
					id3Reader->setUnicodeData("TALB", cLrc.l_al, lstrlen(cLrc.l_al));
				}

				id3Reader->setRawData("USLT", uData, uLen);

				id3Reader->updateID3Data();
				id3Reader->updateMP3File();

				cDlg->lb_progress.SetItemText(i, 0, L"성공");
				
				/* 그룹변경 */
				int gi = cDlg->insertGroup(cDlg->lb_flist, nGroupName);
				lstrcpy(id->lyricsStr, nGroupName);
				id->lyricsState = mp3_Machine;
				LVITEM lvi;
				::ZeroMemory(&lvi, sizeof(lvi));
				lvi.mask =  LVIF_GROUPID;
				lvi.iGroupId = gi;
				lvi.iItem = id->index;
				cDlg->lb_flist.SetItem(&lvi);

				delete uData;
			}
		}

		cDlg->m_progress.SetPos((double)(i+1) / (double)nCnt * 100.0f);
	}

	// finished Job
	cDlg->m_progress.SetPos(100);
	toggleEnable(TRUE, lParam);
	doProcess = false;

	return 0;
}

UINT ExtractMP3Lyrics(LPVOID lParam)
{
	if (doProcess) return 0;
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lParam;
	CID3Reader *id3Reader = new CID3Reader();

	// starting Job
	toggleEnable(FALSE, lParam);
	toggleViewMode(FALSE, lParam);

	// progress
	WCHAR filepath[256];
	int nCnt = cDlg->lb_progress.GetItemCount();

	doProcess = true;
	for (int i=0; i<nCnt; i++)
	{
		/* cancel part */
		if (!doProcess) {
			toggleEnable(TRUE, lParam);
			return 0;
		}
		/* cancel part end */

		cDlg->lb_progress.SetItemText(i, 0, L"->");

		ITEMDATA *id;
		id = (ITEMDATA*)cDlg->lb_progress.GetItemData(i);
		wcscpy(filepath, id->fileName);

		if (!id3Reader->readMP3File(filepath))
		{
			cDlg->lb_progress.SetItemText(i, 0, L"에러");
			cDlg->lb_progress.SetItemText(i, 2, L"올바른 mp3 파일이 아닙니다");
		} else {
			// work
			CLyrics cLrc;

			// get Lyrics
			bool res = false;
			CMD5Maker maker;
			int len, offset = id3Reader->mp3DataStartPos;
			WCHAR MD5Hash[255];

			if (!res && ls_alsong) {
				len = H_ALSONG;
				maker.makeMD5AdvanceW(filepath, MD5Hash, offset, len);
				res = cLrc.getLRCFromAlsongServer(MD5Hash);
				if (!res && b_tagfind)
					res = cLrc.getLRCFromAlsongServerTA(id->s_title, id->s_artist);
			}
			if (!res && ls_gomaudio) {
				len = H_GOMAUDIO;
				maker.makeMD5AdvanceW(filepath, MD5Hash, offset, len);
				res = cLrc.getLRCFromGOMServer(MD5Hash, L"", L"", L"");
			}
			
			if (!res) {
				cDlg->lb_progress.SetItemText(i, 0, L"실패");
				cDlg->lb_progress.SetItemText(i, 2, L"싱크 가사가 없습니다");
			} else {
				cLrc.processLyrics(ls1, ls2, ls3, ls4, false, false);

				WCHAR lrcPath[255];
				wcscpy(lrcPath, filepath);
				int nL = wcslen(lrcPath);
				lrcPath[nL - 3] = 'l';
				lrcPath[nL - 2] = 'r';
				lrcPath[nL - 1] = 'c';

				if (lrcUnicode)
					cLrc.writeLRCFile(lrcPath);
				else
					cLrc.writeLRCFileASCII(lrcPath);

				cDlg->lb_progress.SetItemText(i, 0, L"성공");
			}
		}

		cDlg->m_progress.SetPos((double)(i+1) / (double)nCnt * 100.0f);
	}

	// finished Job
	cDlg->m_progress.SetPos(100);
	toggleEnable(TRUE, lParam);
	doProcess = false;

	return 0;
}

UINT ImportMP3Lyrics(LPVOID lParam)
{
	if (doProcess) return 0;
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lParam;
	CID3Reader *id3Reader = new CID3Reader();

	// starting Job
	toggleEnable(FALSE, lParam);
	toggleViewMode(FALSE, lParam);

	// progress
	WCHAR filepath[256];
	int nCnt = cDlg->lb_progress.GetItemCount();

	doProcess = true;
	for (int i=0; i<nCnt; i++)
	{
		/* cancel part */
		if (!doProcess) {
			toggleEnable(TRUE, lParam);
			return 0;
		}
		/* cancel part end */

		cDlg->lb_progress.SetItemText(i, 0, L"->");

		ITEMDATA *id;
		id = (ITEMDATA*)cDlg->lb_progress.GetItemData(i);
		wcscpy(filepath, id->fileName);

		if (!id3Reader->readMP3File(filepath))
		{
			cDlg->lb_progress.SetItemText(i, 0, L"에러");
			cDlg->lb_progress.SetItemText(i, 2, L"올바른 mp3 파일이 아닙니다");
		} else {
			// work
			CLyrics cLrc;
			WCHAR lrcPath[255];
			wcscpy(lrcPath, filepath);
			int nL = wcslen(lrcPath);
			lrcPath[nL - 3] = 'l';
			lrcPath[nL - 2] = 'r';
			lrcPath[nL - 1] = 'c';

			if (!cLrc.readLRCFile(lrcPath))
			{
				cDlg->lb_progress.SetItemText(i, 0, L"실패");
				cDlg->lb_progress.SetItemText(i, 2, L"lrc 파일이 없습니다");
			} else if (b_remark && id->lyricsState == mp3_Machine) {
				cDlg->lb_progress.SetItemText(i, 0, L"무시");
				cDlg->lb_progress.SetItemText(i, 2, L"이미 동일 기종의 싱크 가사가 존재합니다");
			} else {
				if (mp3_Machine == M_ITUNES)
					cLrc.processLyrics(ls1, ls2, ls3, ls4, false, b_autoWrap);
				else
					cLrc.processLyrics(ls1, ls2, ls3, ls4, true, b_autoWrap);

				BYTE *uData = (BYTE *)malloc(sizeof(BYTE) * LARGEBUF);
				int uLen;
				WCHAR nGroupName[256];

				if (mp3_Machine == M_SAMSUNG) {
					CUSLTSamsung sUSLT;
					sUSLT.Lyrics2SamsungUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_time, cLrc.l_cnt);
					lstrcpy(nGroupName, L"삼성");
				} else if (mp3_Machine == M_IRIVER) {
					CUSLTIriver iUSLT;
					iUSLT.Lyrics2IriverUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_time, cLrc.l_cnt);
					lstrcpy(nGroupName, L"아이리버");
				} else if (mp3_Machine == M_COWON) {
					CUSLTCowon cUSLT;
					strcpy(cUSLT.cowonDevice, cowonDevice);
					cUSLT.Lyrics2CowonUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_time, cLrc.l_cnt);
					CString _buf;
					_buf.Format(L"코원 [%s]", CString(cowonDevice));
					lstrcpy(nGroupName, _buf.GetBuffer());
				} else if (mp3_Machine == M_ITUNES) {
					CUSLTITunes iUSLT;
					iUSLT.Lyrics2ITunesUSLT(uData, &uLen, cLrc.l_lrc, cLrc.l_cnt);
					lstrcpy(nGroupName, L"아이튠스");
				}

				id3Reader->setRawData("USLT", uData, uLen);
				id3Reader->updateID3Data();
				id3Reader->updateMP3File();
				
				/* 그룹변경 */
				int gi = cDlg->insertGroup(cDlg->lb_flist, nGroupName);
				lstrcpy(id->lyricsStr, nGroupName);
				id->lyricsState = mp3_Machine;
				LVITEM lvi;
				::ZeroMemory(&lvi, sizeof(lvi));
				lvi.mask =  LVIF_GROUPID;
				lvi.iGroupId = gi;
				lvi.iItem = id->index;
				cDlg->lb_flist.SetItem(&lvi);

				cDlg->lb_progress.SetItemText(i, 0, L"성공");
			}
		}

		cDlg->m_progress.SetPos((double)(i+1) / (double)nCnt * 100.0f);
	}

	// finished Job
	cDlg->m_progress.SetPos(100);
	toggleEnable(TRUE, lParam);
	doProcess = false;

	return 0;
}


UINT ImportAPIC(LPVOID lParam)
{
	if (doProcess) return 0;
	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lParam;
	CID3Reader *id3Reader = new CID3Reader();

	// starting Job
	toggleEnable(FALSE, lParam);
	toggleViewMode(FALSE, lParam);
	
	// progress
	WCHAR filepath[256];
	int nCnt = cDlg->lb_progress.GetItemCount();

	doProcess = true;
	for (int i=0; i<nCnt; i++)
	{
		/* cancel part */
		if (!doProcess) {
			toggleEnable(TRUE, lParam);
			return 0;
		}
		/* cancel part end */

		cDlg->lb_progress.SetItemText(i, 0, L"->");

		ITEMDATA *id;
		id = (ITEMDATA*)cDlg->lb_progress.GetItemData(i);
		wcscpy(filepath, id->fileName);

		if (lstrlen(id->s_artist)<=0) {
			cDlg->lb_progress.SetItemText(i, 0, L"실패");
			cDlg->lb_progress.SetItemText(i, 2, L"아티스트가 설정되어 있지 않습니다");
		} else if (!id3Reader->readMP3File(filepath))
		{
			cDlg->lb_progress.SetItemText(i, 0, L"에러");
			cDlg->lb_progress.SetItemText(i, 2, L"올바른 mp3 파일이 아닙니다");
		} else {
			// work
			APICSearcher as;
			if (as.getAPIC(id->s_artist, L".\\albumcover.jpg")) {
				id3Reader->loadFile2APIC(L".\\albumcover.jpg");
				id3Reader->updateID3Data();
				id3Reader->updateMP3File();

				/* flist 업데이트 */
				cDlg->lb_flist.SetItemText(id->index, 3, L"존재");

				cDlg->lb_progress.SetItemText(i, 0, L"성공");
			} else {
				cDlg->lb_progress.SetItemText(i, 0, L"실패");
				cDlg->lb_progress.SetItemText(i, 2, L"앨범아트가 없습니다");
			}
		}

		cDlg->m_progress.SetPos((double)(i+1) / (double)nCnt * 100.0f);
	}

	// finished Job
	cDlg->m_progress.SetPos(100);
	toggleEnable(TRUE, lParam);
	doProcess = false;

	return 0;
}

UINT ChangeFrame(LPVOID lParam)
{
	// init
	WCHAR *val = _id3Edit_val;
	char *frameName = _id3Edit_frame;

	CMP3LyricsMasterv003Dlg *cDlg = (CMP3LyricsMasterv003Dlg *)lParam;
	CID3Reader *id3Reader = new CID3Reader();

	// starting Job
	toggleEnable(FALSE, lParam);
	toggleViewMode(FALSE, lParam);

	// progress
	WCHAR filepath[256];
	int nCnt = cDlg->lb_flist.GetItemCount();

	doProcess = true;
	for (int i=0; i<nCnt; i++)
	{
		/* cancel part */
		if (!doProcess) {
			addMessage(L"", L"사용자의 요청으로 작업이 중단되었습니다.", lParam);
			toggleEnable(TRUE, lParam);
			return 0;
		}
		/* cancel part end */

		LVITEM lvi;
		cDlg->lb_flist.GetItem(&lvi);
		wcscpy(filepath, lvi.pszText);

		if (!id3Reader->readMP3File(filepath))
		{
			CString msg;
			msg.Format(L"올바른 mp3 파일이 아닙니다 - %s", filepath);
			addMessage(L"에러", msg, lParam);
		} else {
			// work
			// APIC에서는 최적화가 가능하나, 아직은 방관하겠습니다 -_-;
			if (strcmp(frameName, "APIC") == 0) {
				id3Reader->loadFile2APIC(val);
			} else {
				id3Reader->setUnicodeData(frameName, val, wcslen(val));
			}
		
			id3Reader->updateID3Data();
			id3Reader->updateMP3File();

			CString msg;
			msg.Format(L"%s", filepath);
			addMessage(L"성공", msg, lParam);
		}

		cDlg->m_progress.SetPos((double)i / (double)(nCnt-1) * 100.0f);
	}

	addMessage(L"", L"작업을 마쳤습니다.", lParam);

	// finished Job
	toggleEnable(TRUE, lParam);

	return 0;
}
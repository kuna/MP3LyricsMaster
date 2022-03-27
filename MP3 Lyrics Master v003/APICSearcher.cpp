#include "stdafx.h"
#include "APICSearcher.h"
#include "uniascii.h"

#pragma comment(lib,"WSock32.Lib")
#include <afxsock.h>	// for socket
#include "afxinet.h"

bool APICSearcher::searchAPIC(WCHAR *artist) {
	// CHTTP
	CString url;	// warning! buffer overrun!
	url.Format(L"http://ws.audioscrobbler.com/2.0/?method=artist.getimages&artist=%s&limit=1&api_key=554ae27e0b90079f4fa22a825cf2b200", artist);

	CInternetSession* pSession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);
	CStdioFile* pFile = NULL;
	TCHAR szError[1024] = {0};
	TRY
	{
		pFile = pSession->OpenURL( url, 1, INTERNET_FLAG_TRANSFER_ASCII );
	}
	CATCH( CInternetException, pEx )
	{
		pEx->GetErrorMessage( szError, 1024 );
		return false;
	}
	END_CATCH


	// 2. 이미지 search
	CHttpFile* pHttpFile = (CHttpFile*)pFile;
	char buf[1024], pFormat[30];
	imgSrc[0]=0x00;
	if( pFile != NULL )
	{
		while(pHttpFile->ReadString((LPTSTR)buf, 255))	// HTTP 파일의 끝까지 읽어감
		{
			getBetweenTag(buf, "format", "format", pFormat);
			getBetweenTag(buf, "size name", "size", imgSrc);
			if (strcmp(pFormat, "jpg")==0 && strlen(imgSrc)>0) break;
		}
	}

	pHttpFile->Close();
	pSession->Close();
	delete pHttpFile;
	delete pSession;

	if (strlen(imgSrc)>0) return true;
	else return false;
}

bool APICSearcher::getAPIC(WCHAR *artist, WCHAR *out) {
	if (searchAPIC(artist)) {
		// download file to Out
		CString url(imgSrc);

		CInternetSession* pSession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);
		CStdioFile* pFile = NULL;
		TCHAR szError[1024] = {0};
		TRY
		{
			pFile = pSession->OpenURL( url, 1, INTERNET_FLAG_TRANSFER_ASCII );
		}
		CATCH( CInternetException, pEx )
		{
			pEx->GetErrorMessage( szError, 1024 );
			return false;
		}
		END_CATCH


		// 2. 이미지 search
		CHttpFile* pHttpFile = (CHttpFile*)pFile;
		BYTE *buf=(BYTE*)malloc(1024000);	// 1MB
		int bufLen;

		if( pFile != NULL )
		{
			bufLen = pHttpFile->Read(buf, 1024000);
		}

		pHttpFile->Close();
		pSession->Close();
		delete pHttpFile;
		delete pSession;

		// save to file
		FILE *fp = _tfopen(out, L"wb");
		if (!fp) return false;
		fwrite(buf, 1, bufLen, fp);

		fclose(fp);
		delete buf;

		return true;
	} else return false;
}


// 내부 구현
bool APICSearcher::getBetweenTag(char *org, char *sTag, char *eTag, char *out)
{
	int sl = strlen(org);
	int s=0, e=0;
	for (int i=0; i<sl; i++) {
		if (org[i] == '<' && !s && memcmp(org+i+1, sTag, strlen(sTag))==0) {
			while (org[i] != '>' && i < sl) i++;
			s = i+1;
			continue;
		}
		if (org[i] == '<' && s && !e) {
			if (org[i+1]=='/' && memcmp(org+i+2, eTag, strlen(eTag))==0) {
				e = i;
				break;
			}
			continue;
		}
	}

	if (s && e) {
		memcpy(out, org+s, e-s);
		out[e-s]=0x00;
		return true;
	} else {
		return false;
	}
}
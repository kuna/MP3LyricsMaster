#include "stdafx.h"
#include "lyrics.h"
#include "uniascii.h"

// 참조하기!
// http://blog.naver.com/da10316/70044780129
// [ C++ ] HTML <-> 특수문자간 변환


CLyrics::CLyrics()
{
	wcscpy(l_ar, L"");
	wcscpy(l_ti, L"");
	wcscpy(l_al, L"");
	l_cnt = 0;
	memset(l_time, 0, sizeof(l_time));
}

bool CLyrics::readLRCFile(WCHAR *filepath)
{
	/* by kuna - kuna.wo.tc */
	FILE *fp;
	_wfopen_s(&fp, filepath, L"rb");
	if (fp == NULL) return false;

	// is unicode or ASCII?
	bool isUnicode = false;
	BYTE ch[2];
	fread(ch, 1, 2, fp);
	if (ch[0] == 0xFF && ch[1] == 0xFE) isUnicode = true;
	fseek(fp, 0, SEEK_SET);

	// clear cache before reading
	fflush(stdin);

	char _buf[1024];
	WCHAR buf[1024];
	int msec, sec, ssec;
	int i=0;

	if (isUnicode) fgetws(buf, 1024, fp);
	else fgets(_buf, 1024, fp), DecodetoUtf8(_buf, buf);
	memcpy(l_ar, buf+3, (wcslen(buf)-4)*sizeof(WCHAR));	l_ar[wcslen(buf)-4] = '\0';

	if (isUnicode) fgetws(buf, 1024, fp);
	else fgets(_buf, 1024, fp), DecodetoUtf8(_buf, buf);
	memcpy(l_ti, buf+5, (wcslen(buf)-4)*sizeof(WCHAR));	l_ti[wcslen(buf)-4] = '\0';

	if (isUnicode) fgetws(buf, 1024, fp);
	else fgets(_buf, 1024, fp), DecodetoUtf8(_buf, buf);
	memcpy(l_al, buf+5, (wcslen(buf)-4)*sizeof(WCHAR));	l_al[wcslen(buf)-4] = '\0';

	while (!feof(fp))
	{
		// Unicode Part
		if (isUnicode) fgetws(buf, 1024, fp); else fgets(_buf, 1024, fp), DecodetoUtf8(_buf, buf);

		// ignore wrong data
		if (wcslen(buf) < 10) continue;
		if (buf[0] != '[' || buf[3] != ':' || buf[6] != '.' || buf[9] != ']') continue;

		msec = (buf[1]-'0')*10 + (buf[2]-'0');
		sec = (buf[4]-'0')*10 + (buf[5]-'0');
		ssec = (buf[7]-'0')*10 + (buf[8]-'0');

		int len = wcslen(buf);
		memcpy(buf, buf+10, (len-10)*sizeof(WCHAR));
		buf[len-11] = '\0';		

		l_time[i] = msec*60*100 + sec*100 + ssec;
		l_lrc[i] = buf;
		i++;
	}

	fclose(fp);
	l_cnt = i;

	return true;
}
bool CLyrics::writeLRCFile(WCHAR *filepath)
{
	// Unicode Output - First at 'FF FE'
	FILE *fp;
	_wfopen_s(&fp, filepath, L"wb");
	if (fp == NULL) return false;

	BYTE unicodeS[] = {0xFF, 0xFE};
	fwrite(unicodeS, 1, 2, fp);
	fwprintf(fp, L"[ar:%s]\r\n", l_ar);
	fwprintf(fp, L"[ti:%s]\r\n", l_ti);
	fwprintf(fp, L"[al:%s]\r\n", l_al);

	for (int i=0; i<l_cnt; i++)
	{
		fwprintf(fp, L"[%02d:%02d.%02d]%s\r\n", (int)(l_time[i]/100/60), (int)(l_time[i]/100%60), l_time[i]%100, l_lrc[i].GetBuffer(0));
	}

	fclose(fp);

	return true;
}
bool CLyrics::writeLRCFileASCII(WCHAR *filepath)
{
	// ASCII Output
	FILE *fp;
	_wfopen_s(&fp, filepath, L"wb");
	if (fp == NULL) return false;

	char buf[256];

	Unicode2ASCII(l_ar, buf);
	fprintf(fp, "[ar:%s]\r\n", buf);
	Unicode2ASCII(l_ti, buf);
	fprintf(fp, "[ti:%s]\r\n", buf);
	Unicode2ASCII(l_al, buf);
	fprintf(fp, "[al:%s]\r\n", buf);

	for (int i=0; i<l_cnt; i++)
	{
		Unicode2ASCII(l_lrc[i].GetBuffer(), buf);
		fprintf(fp, "[%02d:%02d.%02d]%s\r\n", (int)(l_time[i]/100/60), (int)(l_time[i]/100%60), l_time[i]%100, buf);
	}

	fclose(fp);

	return true;
}


int CLyrics::getLRCCnt()
{
	return l_cnt;
}
void CLyrics::getLRCHeader(WCHAR *ar, WCHAR *ti, WCHAR *al)
{
	wcscpy(ar, l_ar);
	wcscpy(ti, l_ti);
	wcscpy(al, l_al);
}
void CLyrics::setLRCHeader(WCHAR *ar, WCHAR *ti, WCHAR *al)
{
	wcscpy(l_ar, ar);
	wcscpy(l_ti, ti);
	wcscpy(l_al, al);
}
bool CLyrics::getLRC(int index, WCHAR *lrc, int *time)
{
	// exception
	if (l_cnt == 0 || l_cnt < index || index < 0) return false;

	wcscpy(lrc, l_lrc[index].GetBuffer(0));
	*time = l_time[index];
	return true;
}
bool CLyrics::setLRC(int index, WCHAR *lrc, int time)
{
	// exception
	if (l_cnt == 0 || l_cnt < index || index < 0) return false;

	l_lrc[index] = lrc;
	l_time[index] = time;
	return true;
}
void CLyrics::getLRCAll(int *cnt, CString *lrc, int *time, WCHAR *ar, WCHAR *ti, WCHAR *al)
{
	getLRCHeader(ar, ti, al);
	*cnt = l_cnt;

	for (int i=0; i<l_cnt; i++)
	{
		lrc[i] = l_lrc[i];
		time[i] = l_time[i];
	}
}
int CLyrics::getLRCIndex(int time)
{
	// exception
	if (l_cnt == 0) return -1;
	if (time < l_time[0]) return 0;

	for (int i=0; i<l_cnt-1; i++)
	{
		if (l_time[i] <= time && time < l_time[i+1])
			return i;
	}

	return l_cnt;
}

void CLyrics::clearLRC()
{
	l_cnt = 0;
}
void CLyrics::createLRC(WCHAR *ar, WCHAR *ti, WCHAR *al)
{
	setLRCHeader(ar, ti, al);
	clearLRC();
}
void CLyrics::addLRC(WCHAR *lrc, int time)
{
	l_lrc[l_cnt] = lrc;
	l_time[l_cnt] = time;
	l_cnt++;
}
bool CLyrics::insertLRC(int index, CString lrc, int time)
{
	// exception
	if (l_cnt < index) return false;

	// shift LRC
	for (int i=l_cnt; i>=index; i--)
	{
		l_lrc[i+1] = l_lrc[i];
		l_time[i+1] = l_time[i];
	}

	// add LRC
	l_lrc[index] = lrc;
	l_time[index] = time;
	l_cnt++;

	return true;
}
bool CLyrics::deleteLRC(int index)
{
	// exception
	if (l_cnt < index || l_cnt == 0) return false;

	// shift LRC
	l_cnt--;
	for (int i=index; i<l_cnt; i++)
	{
		l_lrc[i] = l_lrc[i+1];
		l_time[i] = l_time[i+1];
	}

	return true;
}

/* custom function */
bool CLyrics::getLRCFromGOMServer(WCHAR *hash, WCHAR *title, WCHAR *artist, WCHAR *album)
{
	// 1. 가사 얻기
	WCHAR lowerHash[33];
	for ( int i = 0; i < 32; i++)
	{
		lowerHash[i] = towlower(hash[i]);
	}
	lowerHash[32] = '\0';
	
	CString url;	// warning! buffer overrun!
	url.Format(L"http://newlyrics.gomtv.com/cgi-bin/lyrics.cgi?cmd=find_get_lyrics&file_key=%s&title=%s&artist=%s&album=%s&from=gomaudio_local", lowerHash, title, artist, album);

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
	}
	END_CATCH


	// 2. 가사 처리하기
	int index = 0;
	CString ld[1024];
	char buf[255];
	int nCnt;

	CHttpFile* pHttpFile = (CHttpFile*)pFile;
	if( pFile != NULL )
	{
		while(pHttpFile->ReadString((LPTSTR)buf, 255))	// HTTP 파일의 끝까지 읽어감
		{
			WCHAR buffer[255];
			ASCII2Unicode(buf, buffer);
			ld[index] = buffer;
			index++;
		}
	}
	nCnt = index;

	pHttpFile->Close();
	pSession->Close();
	delete pHttpFile;
	delete pSession;

	// 2. 가사가 없으면? return false
	if (nCnt <= 1)
		return false;

	// 3. 가사가 있으면, 본격 Converting 작업 시작
	/* 추후 추가 : ar, ti, al도 . */
	int nlCnt = -1;
	int ns, ne, ts, te;
	WCHAR ltBuf[255], lsBuf[255], lBuf[255];
	int nowTime = 0;
	for (int i=0; i<nCnt; i++)
	{
		ns = ld[i].Find(L"=\"") + 2;
		ne = ld[i].Find(L"\">");
		ts = ne + 2;
		te = ld[i].Find(L"</sync>");

		// is vaild?
		if (ns < 0 || ne < 0 || ts < 0 || te < 0)
			continue;

		// copy data
		wcscpy(lBuf, ld[i].GetBuffer(0));

		memcpy(ltBuf, lBuf+ns, sizeof(WCHAR)*(ne-ns));
		ltBuf[ne-ns] = '\0';
		memcpy(lsBuf, lBuf+ts, sizeof(WCHAR)*(te-ts));
		lsBuf[te-ts] = '\0';

		// &#xxxxx;, &#
		html_entity_decode(lsBuf);

		// 가사를 넣기 전에, 여러 줄일때는 한 줄로 바꾸도록 한다
		nowTime = _wtoi(ltBuf);
		nlCnt++;
		l_time[nlCnt] = nowTime / 10;
		l_lrc[nlCnt] = lsBuf;
	}

	// 4. return true
	l_cnt = nlCnt;

	return true;
}

void CLyrics::html_entity_decode(WCHAR *str)
/* WCHAR Version */
{
	int sLen = wcslen(str);
	for (int i=0; i<sLen-5; i++)
	{
		if (str[i] == '&') {
			if (str[i+1] == '#' && str[i+7] == ';')
			{
				// 숫자 HTML Codes
				// 2Bytes 문자열로 만들고 Decode -> Unicode이므로 ASCII 문자열로
				WCHAR numBuf[6];
				memcpy( numBuf, str+i+2, sizeof(WCHAR)*5 );
				numBuf[5] = '\0';
				int num = _wtoi(numBuf);

				BYTE numByte[2];
				numByte[0] = num / 256 / 256;
				numByte[1] = num / 256 % 256;

				WCHAR wBuf[2];
				memcpy( wBuf, numByte, 2 );
				//wBuf[1] = '\0';

				str[i] = wBuf[0];
				memcpy( str+i+1, str+i+8, sLen-i );
				sLen = wcslen(str);
			} else {
				if (str[i+5] == ';')
				{
					// 4글자 HTML Codes
					WCHAR cs_nbsp[] = L"nbsp";	//' '
					WCHAR cs_quot[] = L"quot";	//"
					WCHAR cs_apos[] = L"apos";	//'

					if (memcmp( str+i+1, cs_nbsp, sizeof(WCHAR)*4 ) == 0)
						str[i] = ' ';
					else if (memcmp( str+i+1, cs_quot, sizeof(WCHAR)*4 ) == 0)
						str[i] = '\"';
					else if (memcmp( str+i+1, cs_apos, sizeof(WCHAR)*4 ) == 0)
						str[i] = '\'';

					memcpy( str+i+1, str+i+6, sLen-i-5 );
					sLen = wcslen(str);
				}
			}
		}
	}
}

bool CLyrics::getLRCFromAlsongServer(WCHAR *hash) 
{
	// make Socket
	if (!AfxSocketInit())
    {
		return false;
    }

	// get Datas
	int port = 80;
	WCHAR url[255];
	wcscpy(url, L"lyrics.alsong.co.kr");
	WCHAR header[1000];

	CString _tmp2;
	_tmp2.Format(L"POST /alsongwebservice/service1.asmx HTTP/1.1\nContent-Type: application/soap+xml; charset=utf-8\nUser-Agent: gSOAP/2.7\nHost: lyrics.alsong.co.kr\nContent-Length: 695\nCache-Control: no-cache\n\n<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:ns2=\"ALSongWebServer/Service1Soap\" xmlns:ns1=\"ALSongWebServer\" xmlns:ns3=\"ALSongWebServer/Service1Soap12\"><SOAP-ENV:Body><ns1:GetLyric5><ns1:stQuery><ns1:strChecksum>%s</ns1:strChecksum><ns1:strVersion>1.93</ns1:strVersion><ns1:strMACAddress>005056C00001</ns1:strMACAddress><ns1:strIPAddress>192.168.1.2</ns1:strIPAddress></ns1:stQuery></ns1:GetLyric5></SOAP-ENV:Body></SOAP-ENV:Envelope>", hash);
	wcscpy(header, _tmp2.GetBuffer());

	CSocket s;

	if( !s.Create() )
	{
		return false;
	}

	if( !s.Connect(url, port) )
	{
		return false;
	}

	char nHeader[1000];
	Unicode2ASCII(header, nHeader);
	s.Send( nHeader, strlen(nHeader) );

	// 데이터 블럭 방지용
	const char _s_identifiler[] = "</soap:Envelope>";

	char aBuf[100000];
	ZeroMemory(aBuf, 100000);
	int ind = 0;
	while (true)
	{
		if (s_recvDelay > 0) ::Sleep(s_recvDelay);
		int r = s.Receive( aBuf+ind, s_recvSize );
		ind += r;
		if (s_recvDelay != 0 && r < s_recvSize) break;
		if (strcmp(aBuf+ind-strlen(_s_identifiler), _s_identifiler) == 0) break;
	}

	// convert UTF8 -> Unicode
	WCHAR decodePacket[100000];
	DecodetoUtf8(aBuf, decodePacket);

	// 아티스트 등 설정
	WCHAR wStatus[10];
	WCHAR wTitle[1000];
	WCHAR wArtist[1000];
	WCHAR wAlbum[1000];
	WCHAR wLyric[100000];

	setTag(wStatus, L"strStatusID", decodePacket);
	if (wStatus[0] != L'1') return false;	// no lyrics

	setTag(wTitle, L"strTitle", decodePacket);
	setTag(wArtist, L"strArtist", decodePacket);
	setTag(wAlbum, L"strAlbum", decodePacket);
	setTag(wLyric, L"strLyric", decodePacket);

	setLRCHeader(wArtist, wTitle, wAlbum);

	CString _buf = wLyric;
	_buf.Replace(L"&lt;br&gt;", L"\n");

	// 줄별 \n으로 나눈다
	int pS = 0, pE;
	bool pEnd = false;
	int nlCnt = -1;
	int nowTime = 0;
	while (!pEnd) {
		pE = _buf.Find(L"\n", pS+1);
		if (pE < 0) {
			pE = _buf.GetLength();
			pEnd = true;
		}

		WCHAR buf[512];
		memcpy(buf, _buf.GetBuffer()+pS, (pE-pS)*sizeof(WCHAR));
		buf[pE-pS] = 0x0000;

		pS = pE+1;

		// get time
		WCHAR tbuf[10];
		tbuf[2] = 0x0000;

		tbuf[0] = buf[1];
		tbuf[1] = buf[2];
		int bi = _wtoi(tbuf);	
		tbuf[0] = buf[4];
		tbuf[1] = buf[5];
		int mi = _wtoi(tbuf);	
		tbuf[0] = buf[7];
		tbuf[1] = buf[8];
		int si = _wtoi(tbuf);	
		// get time end

		// get lyric
		WCHAR lsBuf[512];
		wcscpy(lsBuf, buf+10);
		// get lyric end

		// write
		nowTime = bi*60000 + mi*1000 + si*10;
		nlCnt++;
		l_time[nlCnt] = nowTime / 10;
		l_lrc[nlCnt] = lsBuf;
	}

	l_cnt = nlCnt+1;

	return true;
}

bool CLyrics::setTag(WCHAR *data, WCHAR *tag, WCHAR *dPacket)
{
	int sP, eP;
	for (sP=0; sP<wcslen(dPacket); sP++) {
		if (memcmp(dPacket+sP, tag, sizeof(WCHAR)*wcslen(tag))==0) break;
	}
	if (sP == wcslen(dPacket)) return false;
	for (eP=sP+1; eP<wcslen(dPacket)-1; eP++) {
		if (memcmp(dPacket+eP, tag, sizeof(WCHAR)*wcslen(tag))==0) break;
	}
	sP += wcslen(tag)+1;

	memcpy(data, dPacket+sP, (eP-sP-1)*sizeof(WCHAR));
	data[eP-sP-2] = 0x0000;
	return true;
}

bool CLyrics::getLRCFromAlsongServerTA(WCHAR *strTitle, WCHAR *strArtist)
{
	// make Socket
	if (!AfxSocketInit())
    {
		return false;
    }

	// get Datas
	int port = 80;
	WCHAR url[255];
	wcscpy(url, L"lyrics.alsong.co.kr");
	WCHAR header[1000];

	CStringA _tmp2;
	
	char nTitle[100], nArtist[100];
	UnicodetoUtf8(strTitle, nTitle);
	UnicodetoUtf8(strArtist, nArtist);

	int queryLen = 606+strlen(nTitle)+strlen(nArtist);
	_tmp2.Format("POST /alsongwebservice/service1.asmx HTTP/1.1\n"
"Host: lyrics.alsong.co.kr\n"
"User-Agent: gSOAP/2.7\n"
"Content-Type: application/soap+xml; charset=utf-8\n"
"Content-Length: %d\n"
"Connection: close\n"
"SOAPAction: \"ALSongWebServer/GetResembleLyric2\"\n\n"
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" 
"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:ns2=\"ALSongWebServer/Service1Soap\" xmlns:ns1=\"ALSongWebServer\" xmlns:ns3=\"ALSongWebServer/Service1Soap12\"><SOAP-ENV:Body><ns1:GetResembleLyric2><ns1:stQuery><ns1:strTitle>%s</ns1:strTitle><ns1:strArtistName>%s</ns1:strArtistName><ns1:nCurPage>0</ns1:nCurPage></ns1:stQuery></ns1:GetResembleLyric2></SOAP-ENV:Body></SOAP-ENV:Envelope>",
queryLen, nTitle, nArtist);


	//_tmp2.Format(L"POST /alsongwebservice/service1.asmx HTTP/1.1\nHost: lyrics.alsong.co.kr\nUser-Agent: gSOAP/2.7\nContent-Type: application/soap+xml; charset=utf-8\nContent-Length: %d\nConnection: close\nSOAPAction: \"ALSongWebServer/GetResembleLyric2\"\n\n<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:ns2=\"ALSongWebServer/Service1Soap\" xmlns:ns1=\"ALSongWebServer\" xmlns:ns3=\"ALSongWebServer/Service1Soap12\"><SOAP-ENV:Body><ns1:GetResembleLyric2><ns1:stQuery><ns1:strTitle>%s</ns1:strTitle><ns1:strArtistName>%s</ns1:strArtistName><ns1:nCurPage>0</ns1:nCurPage></ns1:stQuery></ns1:GetResembleLyric2></SOAP-ENV:Body></SOAP-ENV:Envelope>", queryLen, strTitle, strArtist);
	//queryLen = _tmp2.GetLength();

	CSocket s;

	if( !s.Create() )
	{
		return false;
	}

	if( !s.Connect(url, port) )
	{
		return false;
	}

	char nHeader[1000];
	strcpy(nHeader, _tmp2.GetBuffer());

	int len = strlen(nHeader);
	s.Send( nHeader, len );

	// 데이터 블럭 방지용
	const char _s_identifiler[] = "</soap:Envelope>";

	char aBuf[100000];
	ZeroMemory(aBuf, 100000);
	int ind = 0;

	/* exception */
	char except[] = "HTTP/1.1 400 Bad Request";

	while (true)
	{
		if (memcmp(aBuf, except, strlen(except)) == 0) return false;	// DONT CONTINUE
		if (100000 < s_recvSize+ind) break;								// buffer overflow

		if (s_recvDelay > 0) ::Sleep(s_recvDelay);
		int r = s.Receive( aBuf+ind, s_recvSize );
		ind += r;
		if (s_recvDelay != 0 && r < s_recvSize) break;
		if (strcmp(aBuf+ind-strlen(_s_identifiler), _s_identifiler) == 0) break;
	}

	// convert UTF8 -> Unicode
	WCHAR decodePacket[200000];
	DecodetoUtf8(aBuf, decodePacket);

	// 아티스트 등 설정
	WCHAR wLyric[100000];

	if (!setTag(wLyric, L"strLyric", decodePacket)) return false;	// no lyrics

	CString _buf = wLyric;
	_buf.Replace(L"&lt;br&gt;", L"\n");

	// 줄별 \n으로 나눈다
	int pS = 0, pE;
	bool pEnd = false;
	int nlCnt = -1;
	int nowTime = 0;
	while (!pEnd) {
		pE = _buf.Find(L"\n", pS+1);
		if (pE < 0) {
			pE = _buf.GetLength();
			pEnd = true;
		}

		WCHAR buf[512];
		memcpy(buf, _buf.GetBuffer()+pS, (pE-pS)*sizeof(WCHAR));
		buf[pE-pS] = 0x0000;

		pS = pE+1;

		// get time
		WCHAR tbuf[10];
		tbuf[2] = 0x0000;

		tbuf[0] = buf[1];
		tbuf[1] = buf[2];
		int bi = _wtoi(tbuf);	
		tbuf[0] = buf[4];
		tbuf[1] = buf[5];
		int mi = _wtoi(tbuf);	
		tbuf[0] = buf[7];
		tbuf[1] = buf[8];
		int si = _wtoi(tbuf);	
		// get time end

		// get lyric
		WCHAR lsBuf[512];
		wcscpy(lsBuf, buf+10);
		// get lyric end

		// write
		nowTime = bi*60000 + mi*1000 + si*10;
		nlCnt++;
		l_time[nlCnt] = nowTime / 10;
		l_lrc[nlCnt] = lsBuf;
		// write end
	}

	l_cnt = nlCnt+1;

	return true;
}


bool CLyrics::processLyrics( bool ls1, bool ls2, bool ls3, bool depart, bool mp3Mode, bool autoWrap )
{
	int str_len_limit = 62;
	int max_txt_width = 180;
	int time_min_limit = 20; // 100 = 1sec
	int time_max_limit = 200; // 100 = 1sec

	for (int i=0; i<l_cnt; i++) {
		// empty?
		if (l_lrc[i] == L"") {
			deleteLRC(i);
			i--;
			continue;
		}

		// HTML Escape
		l_lrc[i].Replace(L"&nbsp;", L" ");
		l_lrc[i].Replace(L"&lt;", L"<");
		l_lrc[i].Replace(L"&gt;", L">");
		l_lrc[i].Replace(L"&amp;", L"&");
	}

	// multi-line part
	if (!depart) {
		int dup=1;
		for (int i=0; i<l_cnt; i++)
		{
			if (i>0) {
				if (l_time[i-1] != l_time[i])
				{
					int aDup = dup;
					if (aDup >= 3 && !ls3) aDup--;
					if (aDup >= 2 && !ls2) aDup--;
					if (aDup >= 1 && !ls1) aDup--;

					for (int j=1; j<=dup; j++) {
						if (j==1 && aDup <= 1) break;	/* 쓸수 있는 가사가 총 1줄 이하면 어떤 상황에서도 첫줄은 지우지 않음 */

						if (j==1 && ls1) continue;		// 첫 줄 살릴꺼야?

						if (j==2) {
							if (ls2)
								l_lrc[i-dup+1] += L" / " + l_lrc[i-dup+j-1];
						}

						if (j==3) {
							if (ls3)
								l_lrc[i-dup+1] += L" / " + l_lrc[i-dup+j-1];
						}

						deleteLRC(i-dup+j-1), i--;
					}

					// 초기화
					dup = 1;
				} else {
					// same time - multiline
					dup++;
				}
			}
		}
	} else {
		int dup=1;
		for (int i=0; i<l_cnt; i++)
		{
			if (i>0) {
				if (l_time[i-1] != l_time[i])
				{
					int aDup = dup;
					if (aDup >= 3 && !ls3) aDup--;
					if (aDup >= 2 && !ls2) aDup--;
					if (aDup >= 1 && !ls1) aDup--;

					for (int j=1; j<=dup; j++) {
						if (j==1 && aDup <= 1) break;	/* 쓸수 있는 가사가 총 1줄 이하면 어떤 상황에서도 첫줄은 지우지 않음 */

						if (j==1 && ls1) continue;		// 첫 줄 살릴꺼야?

						if (j==2) {
							if (!ls2)
								deleteLRC(i-dup+j-1), i--;
						}

						if (j==3) {
							if (!ls3)
								deleteLRC(i-dup+j-1), i--;
						}
					}

					// 초기화
					dup = 1;
				} else {
					// same time - multiline
					dup++;
				}
			}
		}
	}

	// over-limit, autowrap part (only mp3Mode)
	if (mp3Mode) {
		CFont font;
		font.CreateFont( 12, 0,
			0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY,
			DEFAULT_PITCH,
			L"굴림" );

		CSize size;
		HDC pDC = GetDC(GetDesktopWindow()); 
		SelectObject(pDC, &font ); 

		// 자간 체크
		int cm = 0;
		/*GetTextExtentPoint( pDC, L"aa", 2, &size );
		cm = size.cx;
		GetTextExtentPoint( pDC, L"a", 1, &size );
		cm -= size.cx * 2;*/

		for (int i=0; i<l_cnt; i++) {
			// autowrap part
			if (autoWrap) {
				int wid = 0;
				for (int j=0; j<l_lrc[i].GetLength(); j++) {
					GetTextExtentPoint( pDC, l_lrc[i].Mid(j, 1), 1, &size );
					
					// 한글이면 사이즈 축소
					WCHAR _t = l_lrc[i].Mid(j, 1).GetBuffer()[0];
					if ( _t >= 0xAC00 && _t <= 0xD7A3 )
						size.cx -= 4;	// 한글이면 사이즈 축소

					// 영어면 사이즈 확대
					if ( (_t>=0x0041&&_t<=0x005A) || (_t>=0x0061&&_t<=0x007A) )
						size.cx += 1;

					wid += size.cx + cm;
					if (wid > max_txt_width) {
						CString _ts = l_lrc[i].Right(l_lrc[i].GetLength() - (j-1));
						insertLRC(i+1, _ts, l_time[i]);
						l_lrc[i] = l_lrc[i].Left(j-1);
						break;
					}
				}
			}

			// over limit -> Cut
			if (l_lrc[i].GetLength() > str_len_limit) {
				CString _ts = l_lrc[i].Right(l_lrc[i].GetLength() - str_len_limit);
				insertLRC(i+1, _ts, l_time[i]);
				l_lrc[i] = l_lrc[i].Left(str_len_limit);
			}
		}

		ReleaseDC(GetDesktopWindow(), pDC);
	}

	// autotime part (only MP3 Mode)
	if (mp3Mode) {
		int dup=1;
		for (int i=0; i<l_cnt; i++)
		{
			if (i>0) {
				if (l_time[i-1] != l_time[i])
				{
					int ms = (l_time[i] - l_time[i-1]) / dup;
					if (ms < time_min_limit) ms = time_min_limit;
					if (ms > time_max_limit) ms = time_max_limit;

					for (int j=1; j<dup; j++)
						l_time[i-dup+j] = l_time[i-dup] + j*ms;

					// 초기화
					dup = 1;
				
					// check next time
					if (l_time[i] < l_time[i-1] + ms) l_time[i] = l_time[i-1] + ms;
				} else {
					// same time - multiline
					dup++;
				}
			}
		}
	}

	return true;
}
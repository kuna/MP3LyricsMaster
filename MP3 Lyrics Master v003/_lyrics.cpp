#include "stdafx.h"
#include "lyrics.h"
#include "uniascii.h"

CLyrics::CLyrics()
{
	wcscpy(l_ar, L"");
	wcscpy(l_ti, L"");
	wcscpy(l_al, L"");
	l_cnt = 0;
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
	else fgets(_buf, 1024, fp), ASCII2Unicode(_buf, buf);
	memcpy(l_ar, buf+3, (wcslen(buf)-4)*sizeof(WCHAR));	l_ar[wcslen(buf)-4] = '\0';

	if (isUnicode) fgetws(buf, 1024, fp);
	else fgets(_buf, 1024, fp), ASCII2Unicode(_buf, buf);
	memcpy(l_ti, buf+5, (wcslen(buf)-4)*sizeof(WCHAR));	l_ti[wcslen(buf)-4] = '\0';

	if (isUnicode) fgetws(buf, 1024, fp);
	else fgets(_buf, 1024, fp), ASCII2Unicode(_buf, buf);
	memcpy(l_al, buf+5, (wcslen(buf)-4)*sizeof(WCHAR));	l_al[wcslen(buf)-4] = '\0';

	while (!feof(fp))
	{
		// Unicode Part
		if (isUnicode) fgetws(buf, 1024, fp); else fgets(_buf, 1024, fp), ASCII2Unicode(_buf, buf);

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
bool CLyrics::insertLRC(int index, char *lrc, int time)
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
bool CLyrics::getLRCFromGOMServer(WCHAR *hash, WCHAR *title, WCHAR *artist, WCHAR *album, bool line1, bool line2, bool line3)
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
	int befTime = -1, nowTime = 0, sTime = 1;
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
		if (befTime < nowTime)
		{
			// 다음 배열로 ㄱㄱ
			befTime = nowTime;
			nlCnt++;
			sTime = 1;

			l_time[nlCnt] = nowTime / 10;
			if (sTime == 1) l_lrc[nlCnt] = lsBuf;
		} else {
			// 시간이 동일하다! 가사가 여러 줄일 것이다.
			sTime++;
			if (line2 && sTime == 2)
			{
				if (!line1)
					l_lrc[nlCnt] = L"";		// 첫번째 줄을 넣지 않음
				else
					l_lrc[nlCnt].Append(L" / ");

				l_lrc[nlCnt].Append(lsBuf);
			}
			if (line3 && sTime == 3)
			{
				l_lrc[nlCnt].Append(L" / ");
				l_lrc[nlCnt].Append(lsBuf);
			}
		}
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

bool CLyrics::getLRCFromAlsongServer(WCHAR *hash, bool line1, bool line2, bool line3) 
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
	int befTime = -1, nowTime = 0, sTime = 1;
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
		if (befTime < nowTime)
		{
			// 더 큰 시간은 다음 배열로 ㄱㄱ
			befTime = nowTime;
			nlCnt++;
			sTime = 1;

			l_time[nlCnt] = nowTime / 10;
			if (sTime == 1) l_lrc[nlCnt] = lsBuf;
		} else {
			// 시간이 동일하다! 가사가 여러 줄일 것이다.
			sTime++;
			if (line2 && sTime == 2)
			{
				if (!line1)
					l_lrc[nlCnt] = L"";		// 첫번째 줄을 넣지 않음
				else
					l_lrc[nlCnt].Append(L" / ");

				l_lrc[nlCnt].Append(lsBuf);
			}
			if (line3 && sTime == 3)
			{
				l_lrc[nlCnt].Append(L" / ");
				l_lrc[nlCnt].Append(lsBuf);
			}
		}
		// write end
	}

	l_cnt = nlCnt;
	return true;
}

void CLyrics::setTag(WCHAR *data, WCHAR *tag, WCHAR *dPacket)
{
	int sP, eP;
	for (sP=0; sP<wcslen(dPacket); sP++) {
		if (memcmp(dPacket+sP, tag, sizeof(WCHAR)*wcslen(tag))==0) break;
	}
	for (eP=sP+1; eP<wcslen(dPacket)-1; eP++) {
		if (memcmp(dPacket+eP, tag, sizeof(WCHAR)*wcslen(tag))==0) break;
	}
	sP += wcslen(tag)+1;

	memcpy(data, dPacket+sP, (eP-sP-1)*sizeof(WCHAR));
	data[eP-sP-2] = 0x0000;
}
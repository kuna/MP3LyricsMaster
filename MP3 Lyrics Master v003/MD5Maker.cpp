#include "StdAfx.h"
#include ".\md5maker.h"
#include "uniascii.h"	// for unicode/ascii converting

#define BUFFER_MAX 400000

CMD5Maker::CMD5Maker(void)
{
}

CMD5Maker::~CMD5Maker(void)
{
}


int CMD5Maker::makeMD5s(vector<string>& filepaths, vector<string>& md5s)
{
	_ASSERT(filepaths.size() > 0);
	_ASSERT(md5s.size() == 0);

	int successCnt = 0;
	int cnt = (int)filepaths.size();
	md5s.insert(md5s.begin(), cnt, "");
	for (int i=0; i<cnt; i++)
	{
		if (makeMD5(filepaths[i], md5s[i]))
		{
			successCnt++;
		}
	}

	return successCnt;
}

bool CMD5Maker::makeMD5(string& filepath, string& md5)
{	
	MD5_CTX ctx;
	FILE    *fp;
	BYTE    buffer[4096];
	int     nRead;
	BYTE    MD5Hash[16];
	char	szTmp[4];
	
	fp = fopen(filepath.c_str(), "rb");
	if (!fp)
	{
		ATLTRACE("파일을 열지 못했습니다.\n");
		return false;
	}

	MD5Init(&ctx);

	while((nRead = fread(buffer, 1, sizeof buffer, fp)) > 0)
	{
		MD5Update(&ctx, buffer, nRead);
	}

	MD5Final(MD5Hash, &ctx);

	md5 = "";
	for(int i=0; i<16; ++i)
	{
		sprintf(szTmp, "%02X", MD5Hash[i]);
		md5 += szTmp;
		//printf("%02X", MD5Hash[i]);
	}

	fclose(fp);

	return true;
}

bool CMD5Maker::makeMD5Advance(char *filepath, string& md5, int offset, int length)
{

	MD5_CTX ctx;
	FILE    *fp;
	BYTE    buffer[BUFFER_MAX];
	int     nRead;
	BYTE    MD5Hash[16];
	char	szTmp[4];
	
	fp = fopen(filepath, "rb");
	if (!fp)
	{
		ATLTRACE("파일을 열지 못했습니다.\n");
		return false;
	}

	MD5Init(&ctx);

	fseek(fp, offset, SEEK_SET);
	nRead = fread(buffer, 1, length, fp);
	MD5Update(&ctx, buffer, nRead);

	MD5Final(MD5Hash, &ctx);

	md5 = "";
	for(int i=0; i<16; ++i)
	{
		sprintf(szTmp, "%02X", MD5Hash[i]);
		md5 += szTmp;
		//printf("%02X", MD5Hash[i]);
	}

	fclose(fp);

	return true;
}

// unicode
bool CMD5Maker::makeMD5AdvanceW(WCHAR *filepath, WCHAR *md5, int offset, int length)
{

	MD5_CTX ctx;
	FILE    *fp;
	BYTE    *buffer;
	int     nRead;
	BYTE    MD5Hash[16];
	char	szTmp[4];
	
	fp = _wfopen(filepath, L"rb");
	if (!fp)
	{
		ATLTRACE("파일을 열지 못했습니다.\n");
		return false;
	}

	// malloc buffer
	buffer = (BYTE *)malloc(sizeof(BYTE) * length);

	MD5Init(&ctx);

	fseek(fp, offset, SEEK_SET);
	nRead = fread(buffer, 1, length, fp);
	MD5Update(&ctx, buffer, nRead);

	MD5Final(MD5Hash, &ctx);

	WCHAR wszTmp[4];
	for(int i=0; i<16; ++i)
	{
		sprintf(szTmp, "%02X", MD5Hash[i]);
		ASCII2Unicode(szTmp, wszTmp);
		memcpy(md5 + i*sizeof(WCHAR), wszTmp, sizeof(WCHAR)*2);
	}
	md5[32] = '\0';

	fclose(fp);
	delete buffer;

	return true;
}
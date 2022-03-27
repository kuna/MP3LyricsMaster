// unicode 2 ASCII, ASCII 2 unicode free converter
// by kuna - kuna.wo.tc

#include "uniascii.h"
#include "stdafx.h"

int Unicode2ASCII(WCHAR *uni, char *ascii)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, uni, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, uni, -1, ascii, nLen, 0, 0);
	return nLen;
}
int ASCII2Unicode(char *ascii, WCHAR *uni)
{
	int nLen = MultiByteToWideChar(CP_ACP, 0,ascii, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, ascii, -1, uni, nLen);
	return nLen;
}
int DecodetoUtf8(LPCSTR utf8str, WCHAR *str)
{
	USES_CONVERSION;
	int size = MultiByteToWideChar(CP_UTF8, 0, utf8str, -1, NULL, 0);
	LPWSTR wStr = new WCHAR[size];
	MultiByteToWideChar(CP_UTF8, 0, utf8str, -1, wStr, size);
	wcscpy(str, wStr);
	delete[] wStr;
	return size;
}
int UnicodetoUtf8(WCHAR *utf8str, LPCSTR str)
{
	USES_CONVERSION;
	int size = WideCharToMultiByte(CP_UTF8, 0, utf8str, -1, NULL, 0, 0, 0);
	LPSTR nstr = new CHAR[size];
	WideCharToMultiByte(CP_UTF8, 0, utf8str, -1, nstr, size, 0, 0);
	strcpy((char*)str, nstr);
	delete[] nstr;
	return size;
}
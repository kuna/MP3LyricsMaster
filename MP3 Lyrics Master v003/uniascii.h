// unicode 2 ASCII, ASCII 2 unicode free converter
// by kuna - kuna.wo.tc

int Unicode2ASCII(WCHAR *uni, char *ascii);
int ASCII2Unicode(char *ascii, WCHAR *uni);
int DecodetoUtf8(LPCSTR utf8str, WCHAR *str);
int UnicodetoUtf8(WCHAR  *utf8str, LPCSTR str);
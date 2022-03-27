/* ID3 Tag Reader By KUNA - v2.0 [WCHAR rev] */
/* supports ID3v1, ID3v2.3 */
/* Version :: Unicode */

#ifndef ID3ENGINE_KUNA
#define ID3ENGINE_KUNA

#define COPYBUF 400000
#define LARGEBUF 2000000	// 2000000 - 2MB
#define	SMALLBUF 2000000	// 2MB
#define CSTBUF 10

class CID3Reader
{
public:
	CID3Reader();
	~CID3Reader();

	bool readMP3File(WCHAR *filepath);
	int readLAMEInfo(int foffset, FILE *fp);
	bool writeMP3File(WCHAR *filepath);
	bool updateMP3File();
	void refreshmp3DataStartPos(FILE *fp);

	bool updateID3Data();
	void getID3Data(BYTE *pData, int *pSize);
	int getID3Len();

	bool readID3v22();
	bool readID3v23();	// also ID3v2.4

	bool getRawData(const char *dataType, BYTE *pData, int *size);
	bool setRawData(const char *dataType, BYTE *pData, int size);
	bool deleteFrame(const char *dataType);

	bool getASCIIData(const char *dataType, char *text);
	bool getUnicodeData(const char *dataType, WCHAR *text);
	bool setASCIIData(const char *dataType, char *text, int datalen);
	bool setUnicodeData(const char *dataType, WCHAR *text, int datalen);
	bool setUTF8Data(const char *dataType, WCHAR *text, int datalen);


	bool isDataExists(const char *dataType);
	bool saveData2File(const char *dataType, char *filepath);
	bool loadFile2Data(const char *dataType, char *filepath);
	bool saveAPIC2File(WCHAR *filepath);
	bool loadFile2APIC(WCHAR *filepath);

	bool isID3v1();
private:
	bool isOpen;
	WCHAR openingFile[256];

	BYTE *id3Data;
	int id3Len;
	BYTE id3Ver;

	bool isID3v2Exist;
	bool isID3v1Exist;

	/* types of Data - C:\Program Files\Microsoft Visual Studio 8\VC\include\id3\globals.h */
	/* not all, only typical datas */
	BYTE *d_APIC;			// pic
	BYTE d_COMM[SMALLBUF];	// comment [30]
	BYTE d_TALB[SMALLBUF];	// album [30]
	BYTE d_TIT2[SMALLBUF];	// title [30]
	BYTE d_TPE1[SMALLBUF];	// artist [30];
	BYTE d_TYER[SMALLBUF];	// year [4]
	BYTE d_TRCK[SMALLBUF];	// track
	BYTE d_TCON[SMALLBUF];	// genre [IDv3.2]
	BYTE d_USLT[SMALLBUF];	// uslt Data
	BYTE d_Genre;			// genre

	/* data's Length */
	int l_APIC;
	int l_COMM;
	int l_TALB;
	int l_TIT2;
	int l_TPE1;
	int l_TYER;
	int l_TRCK;
	int l_TCON;
	int l_USLT;

	/* LAME Tag */
	int LAMELen;
	BYTE LAMEData[SMALLBUF];

public:
	/* etc */
	int mp3DataStartPos;
	DWORD fileLen;

private:
	char c_APIC[CSTBUF];
	char c_COMM[CSTBUF];
	char c_TALB[CSTBUF];
	char c_TIT2[CSTBUF];
	char c_TPE1[CSTBUF];
	char c_TYER[CSTBUF];
	char c_TRCK[CSTBUF];
	char c_TCON[CSTBUF];
	char c_USLT[CSTBUF];

	char c_TT2[CSTBUF];	// TIT2
	char c_TP1[CSTBUF];	// TPE1
	char c_TAL[CSTBUF];	// TALB
	char c_COM[CSTBUF];	// TCOM

	void initLength();
	void writeFrameHeader(BYTE *pData, char *name, int size);
	void clearMemory(BYTE *p, int len);

	int _id3Len;		// Tempoarary
};

#endif
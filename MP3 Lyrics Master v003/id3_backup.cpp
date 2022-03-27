#include "stdafx.h"
#include "id3.h"
#include "uniascii.h"	// for uni <-> ascii converting

// more refer at http://php.net/manual/en/ref.id3.php
// id3v2.3 RAWTAG Edit lib By kuna - http://kuna.wo.tc

CID3Reader::CID3Reader()
{
	strcpy(c_APIC, "APIC");
	strcpy(c_COMM, "COMM");
	strcpy(c_TALB, "TALB");
	strcpy(c_TIT2, "TIT2");
	strcpy(c_TPE1, "TPE1");
	strcpy(c_TYER, "TYER");
	strcpy(c_TRCK, "TRCK");
	strcpy(c_USLT, "USLT");

	strcpy(c_TT2, "TT2");
	strcpy(c_TP1, "TP1");
	strcpy(c_TAL, "TAL");
	strcpy(c_COM, "COM");

	isOpen = false;
}

bool CID3Reader::readMP3File(char *filepath)
{
	isOpen = false;
	// init
	clearMemory(d_TIT2, SMALLBUF);
	clearMemory(d_TPE1, SMALLBUF);
	clearMemory(d_TALB, SMALLBUF);
	clearMemory(d_TYER, SMALLBUF);
	clearMemory(d_COMM, SMALLBUF);
	initLength();

	FILE *fp;
	fopen_s(&fp, filepath, "rb");
	if (fp == NULL) return false;
	strcpy(openingFile, filepath);

	// read ID3v1 tag first
	fseek(fp, -128, SEEK_END);
	BYTE id3Tagv1[128];
	fread(id3Tagv1, 1, 128, fp);
	if (id3Tagv1[0] == 'T' && id3Tagv1[1] == 'A' && id3Tagv1[2] == 'G')
	{
		// title part - store as ID3v2 type
		d_TIT2[0] = 0x00;
		memcpy(d_TIT2 + 1, id3Tagv1 + 3, 30);
		l_TIT2 = strlen((char *)(d_TIT2 + 1)) + 1;
		// artist
		d_TPE1[0] = 0x00;
		memcpy(d_TPE1 + 1, id3Tagv1 + 33, 30);
		l_TPE1 = strlen((char *)(d_TPE1 + 1)) + 1;
		// album
		d_TALB[0] = 0x00;
		memcpy(d_TALB + 1, id3Tagv1 + 63, 30);
		l_TALB = strlen((char *)(d_TALB + 1)) + 1;
		// year
		d_TYER[0] = 0x00;
		memcpy(d_TYER + 1, id3Tagv1 + 93, 4);
		l_TYER = strlen((char *)(d_TYER + 1)) + 1;
		// comment
		d_COMM[0] = 0x00;
		memcpy(d_COMM + 1, id3Tagv1 + 97, 30);
		l_COMM = strlen((char *)(d_COMM + 1)) + 1;
		// genre
		d_Genre = id3Tagv1[127];

		isID3v1Exist = true;
	} else {
		isID3v1Exist = false;
	}
	
	// read ID3v2 tag
	fflush(stdin);	// clear buffer!

	fseek(fp, 0, SEEK_SET);
	BYTE id3v2Header[10];
	fread(id3v2Header, 1, 10, fp);
	bool _res = false;
	if (id3v2Header[0] == 'I' && id3v2Header[1] == 'D' && id3v2Header[2] == '3')
	{
		// get length & read raw id3Tag
		id3Len = id3v2Header[6] << 21 | id3v2Header[7] << 14 | id3v2Header[8] << 7 | id3v2Header[9];
		id3Ver = (int)id3v2Header[3];
		fread(id3Data, 1, id3Len, fp);

		// depart for ID3Tag
		if (id3v2Header[3] == 2)
		{
		// ID2v2.2
			id3Ver = 2;
			_res = readID3v22();
		} else if (id3v2Header[3] == 3) {
		// ID2v2.3
			id3Ver = 3;
			_res = readID3v23();
		} else if (id3v2Header[3] == 4) {
		// ID2v2.4 - not sure!
			id3Ver = 4;
			_res = readID3v23();
		} else {
		// unsupported!
			_res = false;
		}
	} else if (isID3v1Exist) {
		// ID3 태그 존재시
		_res = true;
		id3Len = 0;
	} else {
		// 아무 태그도 존재하지 않는다. 이걸 MP3 파일이라고 봐야 하나?
		// 확장자로 확인해보자
		int sLen = strlen(filepath);
		if (filepath[sLen - 3] == 'm' && filepath[sLen - 2] == 'p' && filepath[sLen - 1] == '3') {
			id3Len = 0;
			_res = true;
		} else {
			_res = false;
		}
	}

	fclose(fp);

	if (!_res) {
		return false;
	} else {
		isOpen = true;
		return true;
	}
}
bool CID3Reader::writeMP3File(char *filepath)
{
	if (!isOpen) return false;
	
	FILE *fp, *fpOut;
	fopen_s( &fp, openingFile, "rb" );
	fopen_s( &fpOut, filepath, "wb" );;
	if (fpOut == NULL || fp == NULL) return false;

	// 데이터는 변경되지 않았다는 가정하에 작업을 수행한다.
	// 또, 미리 updateID3Data() 함수를 호출하여 ID3 데이터를 갱신했다는 가정하에 작업을 수행한다.
	// 1. write ID3v2.3 Tag
	fwrite(id3Data, 1, id3Len + 10, fpOut);

	// 2. read Orginal file's Pure data start pos
	int orgDataPos = 0;
	BYTE orgHeader[10];
	fseek(fp, 0, SEEK_SET);
	fread(orgHeader, 1, 10, fp);
	if (orgHeader[0] == 'I' && orgHeader[1] == 'D' && orgHeader[2] == '3')
		orgDataPos = orgHeader[6] << 21 | orgHeader[7] << 14 | orgHeader[8] << 7 | orgHeader[9];

	// 2. Copy Data - Only Data! [IMPORTANT]
	// [to copy only data, we check copy length]
	fseek(fp, 0, SEEK_END);
	DWORD totSize = ftell(fp);
	totSize -= orgDataPos;
	if (isID3v1Exist)
		totSize -= 128;

	fseek(fp, orgDataPos + 10, SEEK_SET);	// exclude ID3 Header
	BYTE buf[COPYBUF];
	DWORD copySize, copiedSize = 0;
	while (true)
	{
		copySize = totSize - copiedSize;
		if (copySize > COPYBUF) copySize = COPYBUF;
		if (copySize == 0) break;

		fread(buf, 1, copySize, fp);
		fwrite(buf, 1, copySize, fpOut);

		copiedSize += copySize;
	}

	// 3. write ID3v1 Data
	BYTE id3v1Header[128];
	clearMemory(id3v1Header, 128);

	id3v1Header[0] = 'T';
	id3v1Header[1] = 'A';
	id3v1Header[2] = 'G';

	int tagSize;
	BYTE tagData[255];

	clearMemory(tagData, 255);
	getASCIIData("TIT2", (char *)tagData);
	memcpy(id3v1Header + 3, tagData, 30);
	clearMemory(tagData, 255);
	getASCIIData("TPE1", (char *)tagData);
	memcpy(id3v1Header + 33, tagData, 30);
	clearMemory(tagData, 255);
	getASCIIData("TALB", (char *)tagData);
	memcpy(id3v1Header + 63, tagData, 30);
	clearMemory(tagData, 255);
	getASCIIData("TYER", (char *)tagData);
	memcpy(id3v1Header + 93, tagData, 30);
	clearMemory(tagData, 255);
	getASCIIData("COMM", (char *)tagData);
	memcpy(id3v1Header + 97, tagData, 30);
	id3v1Header[127] = d_Genre;

	fwrite(id3v1Header, 1, 128 , fpOut);

	/* the END!! */
	fclose(fp);
	fclose(fpOut);
	return true;
}
bool CID3Reader::updateMP3File()
{
	if (!isOpen) return false;

	char newPath[255];	strcpy(newPath, openingFile);	strcat(newPath, "_");
	writeMP3File(newPath);

	// remove 'n rename
	remove(openingFile);
	rename(newPath, openingFile);

	return true;
}
bool CID3Reader::updateID3Data()
{
	if (!isOpen) return false;

	// update ID3 Data
	// 1. set Default Header
	id3Data[0] = 0x49;
	id3Data[1] = 0x44;
	id3Data[2] = 0x33;
	id3Data[3] = 0x03;	// ID3 Ver 2.3
	id3Data[4] = 0x00;
	id3Data[5] = 0x00;

	// 2. start writing frame datas
	int nPos = 10;
	if (l_APIC > 0) {	// under LARGEBUF - buffer overrun
		writeFrameHeader(id3Data + nPos, c_APIC, l_APIC);
		memcpy(id3Data + nPos + 10, d_APIC, l_APIC);
		nPos += l_APIC + 10;
	}
	if (l_COMM > 1) {	// string should over 1
		writeFrameHeader(id3Data + nPos, c_COMM, l_COMM);
		memcpy(id3Data + nPos + 10, d_COMM, l_COMM);
		nPos += l_COMM + 10;
	}
	if (l_TALB > 1) {	// string should over 1
		writeFrameHeader(id3Data + nPos, c_TALB, l_TALB);
		memcpy(id3Data + nPos + 10, d_TALB, l_TALB);
		nPos += l_TALB + 10;
	}
	if (l_TIT2 > 1) {	// string should over 1
		writeFrameHeader(id3Data + nPos, c_TIT2, l_TIT2);
		memcpy(id3Data + nPos + 10, d_TIT2, l_TIT2);
		nPos += l_TIT2 + 10;
	}
	if (l_TPE1 > 1) {	// string should over 1
		writeFrameHeader(id3Data + nPos, c_TPE1, l_TPE1);
		memcpy(id3Data + nPos + 10, d_TPE1, l_TPE1);
		nPos += l_TPE1 + 10;
	}
	if (l_TYER > 1) {	// string should over 1
		writeFrameHeader(id3Data + nPos, c_TYER, l_TYER);
		memcpy(id3Data + nPos + 10, d_TYER, l_TYER);
		nPos += l_TYER + 10;
	}
	if (l_TRCK > 1) {
		writeFrameHeader(id3Data + nPos, c_TRCK, l_TRCK);
		memcpy(id3Data + nPos + 10, d_TRCK, l_TRCK);
		nPos += l_TRCK + 10;
	}
	if (l_USLT > 0) {
		writeFrameHeader(id3Data + nPos, c_USLT, l_USLT);
		memcpy(id3Data + nPos + 10, d_USLT, l_USLT);
		nPos += l_USLT + 10;
	}
	
	// 3. refresh ID3 Len - excluding ID3 Header
	id3Len = nPos - 10;
	id3Data[6] = (BYTE)((BYTE)(id3Len >> 21) >> 1);
	id3Data[7] = (BYTE)((BYTE)(id3Len >> 13) >> 1);
	id3Data[8] = (BYTE)((BYTE)(id3Len >> 6) >> 1);
	id3Data[9] = (BYTE)((BYTE)(id3Len << 1) >> 1);

	return true;
}
void CID3Reader::getID3Data(BYTE *pData, int *pSize)
{
	memcpy(pData, id3Data, id3Len);
	*pSize = id3Len;
}
int CID3Reader::getID3Len()
{
	if (!isOpen) return -1;
	return id3Len;
}

bool CID3Reader::readID3v22()
{
	//if (!isOpen) return false;

	int nPos = 0;
	int tagSize;

	while (true) {
		// exception or EOF
		if (id3Len < nPos) break;
		if (id3Data[nPos] == 0x00) break;

		tagSize = (int)id3Data[nPos+3]*(256^2) + (int)id3Data[nPos+4]*256 + (int)id3Data[nPos+5];
		if (tagSize > id3Len - nPos) tagSize = id3Len - nPos;	// id3Size must smaller than id3Len

		if (memcmp(id3Data + nPos, c_TT2, 3) == 0)		// TT2
		{
			memcpy(d_TIT2, id3Data + nPos + 6, tagSize);
			l_TIT2 = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_TP1, 3) == 0)	// TP1
		{
			memcpy(d_TPE1, id3Data + nPos + 6, tagSize);
			l_TPE1 = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_TAL, 3) == 0)	// TAL
		{
			memcpy(d_TALB, id3Data + nPos + 6, tagSize);
			l_TALB = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_COM, 3) == 0)	// COM
		{
			memcpy(d_COMM, id3Data + nPos + 6, tagSize);
			l_COMM = tagSize;
		}

		nPos += tagSize + 6;
	}

	return true;
}
bool CID3Reader::readID3v23()
{
	//if (!isOpen) return false;

	int nPos = 0;
	int tagSize;

	while (true) {
		// exception or EOF
		if (id3Len < nPos) break;
		if (id3Data[nPos] == 0x00) break;

		tagSize = (int)id3Data[nPos+4]*(256^3) + (int)id3Data[nPos+5]*(256^2) + (int)id3Data[nPos+6]*256 + (int)id3Data[nPos+7];
		if (tagSize > id3Len - nPos) tagSize = id3Len - nPos;	// id3Size must smaller than id3Len

		if (memcmp(id3Data + nPos, c_APIC, 4) == 0)			// APIC
		{
			memcpy(d_APIC, id3Data + nPos + 10, tagSize);
			l_APIC = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_COMM, 4) == 0)	// COMM
		{
			memcpy(d_COMM, id3Data + nPos + 10, tagSize);
			l_COMM = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_TALB, 4) == 0)	// TALB
		{
			memcpy(d_TALB, id3Data + nPos + 10, tagSize);
			l_TALB = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_TIT2, 4) == 0)	// TIT2
		{
			memcpy(d_TIT2, id3Data + nPos + 10, tagSize);
			l_TIT2 = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_TPE1, 4) == 0)	// TPE1
		{
			memcpy(d_TPE1, id3Data + nPos + 10, tagSize);
			l_TPE1 = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_TYER, 4) == 0)	// TYER
		{
			memcpy(d_TYER, id3Data + nPos + 10, tagSize);
			l_TYER = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_TRCK, 4) == 0)	// TRCK
		{
			memcpy(d_TRCK, id3Data + nPos + 10, tagSize);
			l_TRCK = tagSize;
		}
		else if (memcmp(id3Data + nPos, c_USLT, 4) == 0)	// USLT
		{
			memcpy(d_USLT, id3Data + nPos + 10, tagSize);
			l_USLT = tagSize;
		}

		nPos += tagSize + 10;
	}

	return true;
}

bool CID3Reader::getRawData(const char *dataType, BYTE *pData, int *size)
{
	if (!isOpen) return false;

	if (memcmp(c_APIC, dataType, 4) == 0) {
		memcpy(pData, d_APIC, l_APIC);
		*size = l_APIC;
	}
	else if (memcmp(c_COMM, dataType, 4) == 0) {
		memcpy(pData, d_COMM, l_COMM);
		*size = l_COMM;
	}
	else if (memcmp(c_TALB, dataType, 4) == 0) {
		memcpy(pData, d_TALB, l_TALB);
		*size = l_TALB;
	}
	else if (memcmp(c_TIT2, dataType, 4) == 0) {
		memcpy(pData, d_TIT2, l_TIT2);
		*size = l_TIT2;
	}
	else if (memcmp(c_TPE1, dataType, 4) == 0) {
		memcpy(pData, d_TPE1, l_TPE1);
		*size = l_TPE1;
	}
	else if (memcmp(c_TYER, dataType, 4) == 0) {
		memcpy(pData, d_TYER, l_TYER);
		*size = l_TYER;
	}
	else if (memcmp(c_TRCK, dataType, 4) == 0) {
		memcpy(pData, d_TRCK, l_TRCK);
		*size = l_TRCK;
	}
	else if (memcmp(c_USLT, dataType, 4) == 0) {
		memcpy(pData, d_USLT, l_USLT);
		*size = l_USLT;
	}
	else {
		return false;
	}

	return true;
}
bool CID3Reader::setRawData(const char *dataType, BYTE *pData, int size)
{
	if (!isOpen) return false;

	if (memcmp(c_APIC, dataType, 4) == 0) {
		memcpy(d_APIC, pData, size);
		l_APIC = size;
	}
	else if (memcmp(c_COMM, dataType, 4) == 0) {
		memcpy(d_COMM, pData, size);
		l_COMM = size;
	}
	else if (memcmp(c_TALB, dataType, 4) == 0) {
		memcpy(d_TALB, pData, size);
		l_TALB = size;
	}
	else if (memcmp(c_TIT2, dataType, 4) == 0) {
		memcpy(d_TIT2, pData, size);
		l_TIT2 = size;
	}
	else if (memcmp(c_TPE1, dataType, 4) == 0) {
		memcpy(d_TPE1, pData, size);
		l_TPE1 = size;
	}
	else if (memcmp(c_TYER, dataType, 4) == 0) {
		memcpy(d_TYER, pData, size);
		l_TYER = size;
	}
	else if (memcmp(c_TRCK, dataType, 4) == 0) {
		memcpy(d_TRCK, pData, size);
		l_TRCK = size;
	}
	else if (memcmp(c_USLT, dataType, 4) == 0) {
		memcpy(d_USLT, pData, size);
		l_USLT = size;
	}
	else {
		return false;
	}

	return true;
}
bool CID3Reader::deleteFrame(const char *dataType)
{
	if (!isOpen) return false;

	if (memcmp(c_APIC, dataType, 4) == 0)
		l_APIC = 0;
	else if (memcmp(c_COMM, dataType, 4) == 0)
		l_COMM = 0;
	else if (memcmp(c_TALB, dataType, 4) == 0)
		l_TALB = 0;
	else if (memcmp(c_TIT2, dataType, 4) == 0)
		l_TIT2 = 0;
	else if (memcmp(c_TPE1, dataType, 4) == 0)
		l_TPE1 = 0;
	else if (memcmp(c_TYER, dataType, 4) == 0)
		l_TYER = 0;
	else if (memcmp(c_TRCK, dataType, 4) == 0)
		l_TRCK = 0;
	else if (memcmp(c_USLT, dataType, 4) == 0)
		l_USLT = 0;
	else
		return false;

	return true;
}


bool CID3Reader::getASCIIData(const char *dataType, char *text)
{
	if (!isOpen) return false;
	int pLen;
	BYTE pData[SMALLBUF];
	if (!getRawData(dataType, pData, &pLen))
		return false;
	if (pLen == 0)
		return false;

	//00 : ASCII
	if (pData[0] == 0x00)
	{
		memcpy(text, pData + 1, pLen - 1);
		text[pLen-1] = '\0';
	} else {
	///그 외[ex:01] : Unicode
		WCHAR buf[255];
		memcpy(buf, pData + 1, pLen - 1);
		buf[(pLen-1)/2] = '\0';

		Unicode2ASCII(buf+1, text);
	}
	
	return true;
}
bool CID3Reader::getUnicodeData(const char *dataType, WCHAR *text)
{
	if (!isOpen) return false;
	int pLen;
	BYTE pData[SMALLBUF];
	if (!getRawData(dataType, pData, &pLen))
		return false;

	//00 : ASCII
	if (pData[0] == 0x00)
	{
		char buf[SMALLBUF];
		memcpy(text, pData + 1, pLen - 1);
		text[pLen - 1] = '\0';

		ASCII2Unicode(buf, text);
	} else {
	///그 외[ex:01] : Unicode
		memcpy(text, pData + 1, pLen - 1);
		text[(pLen-1)/2] = '\0';
	}
	
	return true;
}
bool CID3Reader::setASCIIData(const char *dataType, char *text, int datalen)
{
	if (!isOpen) return false;

	BYTE pData[SMALLBUF];
	pData[0] = 0x00;
	memcpy(pData + 1, text, datalen);
	if (!setRawData(dataType, pData, datalen + 1))
		return false;

	return true;
}
bool CID3Reader::setUnicodeData(const char *dataType, WCHAR *text, int datalen)
{
	if (!isOpen) return false;

	BYTE pData[SMALLBUF];
	pData[0] = 0x01;
	memcpy(pData + 1, text, datalen);
	if (!setRawData(dataType, pData, datalen + 1))
		return false;

	return true;
}
bool CID3Reader::isDataExists(const char *dataType)
{
	BYTE trashbuf[LARGEBUF];
	int size;
	if (getRawData(dataType, trashbuf, &size))
		if (size > 0)
			return true;

	return false;
}
bool CID3Reader::saveData2File(const char *dataType, char *filepath)
{
	BYTE buf[LARGEBUF];
	int fileSize;

	if (!getRawData(dataType, buf, &fileSize))
		return false;

	FILE *fp;
	fopen_s(&fp, filepath, "wb");
	if (fp == NULL) return false;

	fwrite(buf, 1, fileSize, fp);
	fclose(fp);

	return true;
}
bool CID3Reader::loadFile2Data(const char *dataType, char *filepath)
{
	FILE *fp;
	fopen_s(&fp, filepath, "rb");
	if (fp == NULL) return false;

	BYTE buf[LARGEBUF];

	fseek(fp, 0, SEEK_END);
	DWORD fileSize = ftell(fp);
	if (fileSize > LARGEBUF) return false;	// overflow

	fseek(fp, 0, SEEK_SET);
	fread(buf, 1, fileSize, fp);
	fclose(fp);

	if (!setRawData(dataType, buf, fileSize))
		return false;

	return true;
}
bool CID3Reader::saveAPIC2File(char *filepath)
{
	
	BYTE buf[LARGEBUF];
	int fileSize;

	if (!getRawData("APIC", buf, &fileSize))
		return false;

	FILE *fp;
	fopen_s(&fp, filepath, "wb");
	if (fp == NULL) return false;

	fwrite(buf + 14, 1, fileSize - 14, fp);	// exclude 14 bytes
	fclose(fp);
	/*
	int extPos;
	for (extPos=10; extPos<l_APIC; extPos++)
	{
		if (d_APIC[extPos] == 0x00) break;
	}*/

	//fwrite(d_APIC + 14, 1, l_APIC - 14, fp);	// exclude 14 bytes
	//fclose(fp);

	return true;
}
bool CID3Reader::loadFile2APIC(char *filepath)
{
	FILE *fp;
	fopen_s(&fp, filepath, "rb");
	if (fp == NULL) return false;

	BYTE buf[LARGEBUF];

	fseek(fp, 0, SEEK_END);
	DWORD fileSize = ftell(fp);
	if (fileSize > LARGEBUF) return false;	// overflow

	fseek(fp, 0, SEEK_SET);
	fread(buf + 14, 1, fileSize, fp);
	fclose(fp);

	// make ID3 APIC Header
	buf[0] = 0x00;
	buf[1] = 0x69;
	buf[2] = 0x6D;
	buf[3] = 0x61;
	buf[4] = 0x67;
	buf[5] = 0x65;
	buf[6] = 0x2F;
	buf[7] = 0x6A;
	buf[8] = 0x70;
	buf[9] = 0x65;
	buf[10] = 0x67;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x00;
	fileSize += 14;

	if (!setRawData("APIC", buf, fileSize))
		return false;

	return true;
}

void CID3Reader::initLength()
{
	l_APIC = 0;
	l_COMM = 0;
	l_TALB = 0;
	l_TIT2 = 0;
	l_TPE1 = 0;
	l_TYER = 0;
	l_TRCK = 0;
	l_USLT = 0;
}
void CID3Reader::writeFrameHeader(BYTE *pData, char *name, int size)
{
	// set Header
	pData[0] = name[0];					// tag string
	pData[1] = name[1];
	pData[2] = name[2];
	pData[3] = name[3];

	pData[4] = (BYTE)(size / 256 / 256 / 256 % 256);
	pData[5] = (BYTE)(size / 256 / 256 % 256);
	pData[6] = (BYTE)(size / 256 % 256);
	pData[7] = (BYTE)(size % 256);
	pData[8] = 0x00;						// flags
	pData[9] = 0x00;
}
void CID3Reader::clearMemory(BYTE *p, int len)
{
	for (int i=0; i<len; i++)
		p[i] = 0x00;
}
bool CID3Reader::isID3v1()
{
	return isID3v1Exist;
}
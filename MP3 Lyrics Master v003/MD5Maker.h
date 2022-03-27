#pragma once
//====================================INCLUDES======================================
// System Includes
#include <vector>
#include <string>
using namespace std;
// Project Includes
// Other Project Includes
#include "md5c.h"

//====================================DEFINES=======================================
//====================================STRUCTS=======================================
//===============================GLOBAL VARIABLES===================================
//===============================CLASS DEFINITIONS==================================
/**
*	@class		
*	@brief		
*	@author		
*	@version		
*	@date		
*	@bug		
*
*	@par		Revision:
*				
*				
*
*	@par		Description:
*/
class CMD5Maker
{
public:
	CMD5Maker(void);
	~CMD5Maker(void);


	int makeMD5s(vector<string>& filepaths, vector<string>& md5s);
	bool makeMD5(string& filepath, string& md5);
	bool makeMD5Advance(char *filepath, string& md5, int offset, int length);
	bool makeMD5AdvanceW(WCHAR *filepath, WCHAR *md5, int offset, int length);

private:
};

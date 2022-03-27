#include <iostream>
#include <sstream>
#include "curl/curl.h"
#pragma comment(lib, "libcurl")

#include "iconv.h"
#pragma comment(lib, "iconv")

#include "uniascii.h"
 
#define END_POINT "http://twitpic.com/api/uploadAndPost"
#define EXPECT_ARGS 4

/*
size_t write_data(void *buffer, size_t size, size_t nmemb, void* userp)
{
	std::stringstream strmResponse;
	size_t nReal = size * nmemb;
	strmResponse << std::string((char*)buffer, size*nmemb);
	std::string sLine("");
	while (getline(strmResponse, sLine)) {
		std::cout << sLine.c_str() << std::endl;
	}
	return nReal;
}
*/

bool utf16_to_utf8(CStringW in, std::string& out)
{
	char tt[255];
	UnicodetoUtf8(in.GetBuffer(), tt);
	out = tt;
	return true;
}

int twitPic_UploadW(CStringW sUser, CStringW sPass, CStringW sMessage, CStringW sFileName)
{
	int result = 0;
	CURL* hCurl = NULL;
	CURLcode hResult;

	//Curl for form data
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;

	// Encode
	std::string utf8Msg;
	utf16_to_utf8( sMessage, utf8Msg );

	try {
		//Initialize curl, just don't let easy_init to do it for you
		curl_global_init(CURL_GLOBAL_ALL);
		//Handle to the curl
		hCurl = curl_easy_init();
		if(NULL == hCurl) {
			throw false;
		}

		//Construct the form
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "username", CURLFORM_COPYCONTENTS, CStringA(sUser).GetBuffer(), CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "password", CURLFORM_COPYCONTENTS,  CStringA(sPass).GetBuffer(), CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "message", CURLFORM_COPYCONTENTS, utf8Msg.c_str(), CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "media", CURLFORM_FILE, CStringA(sFileName).GetBuffer(), CURLFORM_END);
		//Specify the API Endpoint
		hResult = curl_easy_setopt(hCurl, CURLOPT_URL, END_POINT);
		//Specify the HTTP Method
		hResult = curl_easy_setopt(hCurl, CURLOPT_HTTPPOST, post);

		//hResult = curl_easy_setopt(hCurl, CURLOPT_WRITEDATA, fp); <- Something different ness.,.

		//Post Away !!!
		hResult = curl_easy_perform(hCurl);

		if(hResult != CURLE_OK){
			std::cout << "Cannot find the twitpic site " << std::endl;
			throw false;
		}
	} catch (...) {
		result = -1;
	}

	
    double statDouble ; 
    long statLong ; 
    char* statString = NULL ; 

    // HTTP 응답코드를 얻어온다.  
    curl_easy_getinfo( hCurl , CURLINFO_HTTP_CODE , &statLong );
 
    // Content-Type 를 얻어온다. 
	curl_easy_getinfo( hCurl , CURLINFO_CONTENT_TYPE , &statString );
 
    // 다운로드한 문서의 크기를 얻어온다. 
	curl_easy_getinfo( hCurl , CURLINFO_SIZE_DOWNLOAD , &statDouble );
 
    //  
	curl_easy_getinfo( hCurl , CURLINFO_SPEED_DOWNLOAD , &statDouble );
 

	curl_formfree(post);
    curl_easy_cleanup(hCurl);
	curl_global_cleanup();
    return result;
}
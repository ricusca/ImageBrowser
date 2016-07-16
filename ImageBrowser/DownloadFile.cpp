#include "DownloadFile.h"
#include "Request.h"
#include "HttpClient.h"
#include "json/json.h"
#include <iostream>

DownloadFile::DownloadFile()
	:mstate(DOWNLOAD_REQUIRED)
{
}


DownloadFile::~DownloadFile()
{
}

void DownloadFile::Download(const char* url)
{
	if (mstate == DOWNLOAD_REQUIRED)
	{
		IRequest* r = new Request(this);
		r->SetUrl(url);
		HttpClient::GetInstance()->SubmitRequest(r);
		mstate = DOWNLOADING;
	}
}

void DownloadFile::HttpSuccess(const char *pData, unsigned int uDataSize)
{
	mData = new char[uDataSize];
	memcpy_s(mData, uDataSize, pData, uDataSize);
	mstate = IN_MEMORY;

	Json::Value root;
	Json::Reader reader;
	bool bResult = reader.parse(pData, pData + uDataSize*sizeof(char), root);
	if (bResult == false)
	{
		std::cout << "Failed to parse response\n"
			<< reader.getFormattedErrorMessages() << std::endl << pData << std::endl;
		return;
	}

	std::string title = root.get("title","My list").asString();
	std::string baseUrl = root.get("baseUrl","!!!empty url!!!").asString();

	std::cout << title << std::endl << baseUrl << std::endl;

	const Json::Value images = root["images"];
	for (size_t idx = 0; idx < images.size(); ++idx)
		std::cout << images[idx].asString() << std::endl;


	//std::cout << pData << std::endl;
}

void DownloadFile::HttpFailure(int iCode, const char* msg)
{
	fprintf(stderr, "curl_easy_perform() failed: %s\n",msg);

}
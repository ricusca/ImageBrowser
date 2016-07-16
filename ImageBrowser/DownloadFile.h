#pragma once
#include "HttpCallback.h"
#include "defines.h"
#include <string>

class DownloadFile :public HttpCallback
{
public:
	DownloadFile();
	~DownloadFile();


	void Download(const char* url);

	// HttpCallback
public:
	virtual void HttpSuccess(const char *pData, unsigned int uDataSize) override;

	virtual void HttpFailure(int iCode, const char* msg) override;

private:
	std::string mFileName;
	char* mData;
	unsigned int mSize;
	FileState mstate;
};


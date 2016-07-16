#include "DownloadFile.h"
#include "Request.h"
#include "HttpClient.h"
#include <iostream>

DownloadFile::DownloadFile()
	:mstate(FileState::DOWNLOAD_REQUIRED)
	, mFileName()
	, mFullUrl()
	, mData(nullptr)
	, mSize(0)
	, request(nullptr)
{

}

DownloadFile::DownloadFile(const char* pFileName, const char* pFullUrl)
	:mstate(FileState::DOWNLOAD_REQUIRED)
	, mFileName(pFileName)
	, mFullUrl(pFullUrl)
	, mData(nullptr)
	, mSize(0)
	, request(nullptr)
{
}

DownloadFile::DownloadFile( DownloadFile& file)
{
	mstate = file.mstate;
	mFileName = file.mFileName;
	mFullUrl = file.mFullUrl;
	mData = file.mData; //yes, don't make a new one, share it
	mSize = file.mSize;
}

DownloadFile& DownloadFile::operator=(DownloadFile file)
{
	Swap(*this,file);

	return *this;
}

//copy and swap idiom, not very useful here
void DownloadFile::Swap(DownloadFile& first, DownloadFile& second)
{
	using std::swap;

	swap(first.mstate, second.mstate);
	swap(first.mFileName, second.mFileName);
	swap(first.mFullUrl, second.mFullUrl);
	swap(first.mData, second.mData);
	swap(first.mSize, second.mSize);
}

DownloadFile::~DownloadFile()
{
	delete [] mData;
	mData = nullptr;

	delete request;
	request = nullptr;
}

void DownloadFile::Download()
{
	if (mstate == FileState::DOWNLOAD_REQUIRED)
	{
		if (request == nullptr)
		{
			request = new Request(this);
		}
		request->SetUrl(mFullUrl.c_str());
		HttpClient::GetInstance()->SubmitRequest(request);
		mstate = FileState::DOWNLOADING;
	}
}

bool DownloadFile::IsReadyForOpen() const
{
	return mstate == FileState::IN_MEMORY;
}

void DownloadFile::GetData(char** pData, uint32_t& dataSize) const
{
	*pData = mData;
	dataSize = mSize;
}

const std::string& DownloadFile::GetName() const
{
	return mFileName;
}

void DownloadFile::SetFullURL(const char* pURL)
{
	mFullUrl = pURL;
}

void DownloadFile::Free()
{
	mstate = FileState::IS_FREE;
}

void DownloadFile::HttpSuccess(const char *pData, uint32_t uDataSize)
{
	mData = new char[uDataSize];
	memcpy_s(mData, uDataSize, pData, uDataSize);
	mSize = uDataSize;
	mstate = FileState::IN_MEMORY;

	//std::cout << pData << std::endl;
}

void DownloadFile::HttpFailure(int32_t iCode, const char* msg)
{
	std::cerr << "curl_easy_perform() failed: " << msg;

	mstate = FileState::FAILED_DOWNLOAD;

}
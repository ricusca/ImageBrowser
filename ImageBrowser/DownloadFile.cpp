#include "DownloadFile.h"
#include "Request.h"
#include "HttpClient.h"

#include "log.h"

DownloadFile::DownloadFile()
	:mstate(FileState::DOWNLOAD_REQUIRED)
	, mFileName()
	, mFullUrl()
	, mData(nullptr)
	, mSize(0)
	, mRequest(nullptr)
	, mMaxRetries(3)
	, mRetries(0)
{

}

DownloadFile::DownloadFile(const char* pFileName, const char* pFullUrl)
	:mstate(FileState::DOWNLOAD_REQUIRED)
	, mFileName(pFileName)
	, mFullUrl(pFullUrl)
	, mData(nullptr)
	, mSize(0)
	, mRequest(nullptr)
	, mMaxRetries(3)
	, mRetries(0)
{
}

DownloadFile::DownloadFile(DownloadFile&& file)
	:mstate(std::move(file.mstate))
	, mFileName(std::move(file.mFileName))
	, mFullUrl(std::move(file.mFullUrl))
	, mData(std::move(file.mData))
	, mSize(std::move(file.mSize))
	, mRequest(std::move(file.mRequest))
	, mMaxRetries(std::move(file.mMaxRetries))
	, mRetries(std::move(file.mRetries))
{

}

DownloadFile::~DownloadFile()
{
	Clean();
}

void DownloadFile::Download( )
{
	if (mstate == FileState::DOWNLOAD_REQUIRED || mstate == FileState::IS_FREE)
	{
		if (mRequest == nullptr)
		{
			mRequest = new Request(this);
		}
		mRequest->SetUrl(mFullUrl.c_str());
		HttpClient::GetInstance()->SubmitRequest(mRequest);
		mstate = FileState::DOWNLOADING;
	}
}

void DownloadFile::RetryDownload()
{
	FILE_LOG(logDEBUG) << "Retry download for " << mFileName;
	if (mRequest != nullptr && mRetries < mMaxRetries)
	{
		++mRetries;
		HttpClient::GetInstance()->SubmitRequest(mRequest);
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
	if (mstate == FileState::IN_MEMORY)
	{
		Clean();
		mstate = FileState::IS_FREE;

		FILE_LOG(logDEBUG) << "Image data for  " << GetName() << " was deleted";
	}
}

void DownloadFile::Clean()
{
	delete[] mData;
	mData = nullptr;

	delete mRequest;
	mRequest = nullptr;
}

void DownloadFile::HttpSuccess(const char *pData, uint32_t uDataSize)
{
	FILE_LOG(logINFO) << "Download succeeded for: " << mFileName;

	mData = new char[uDataSize];
	memcpy_s(mData, uDataSize, pData, uDataSize);
	mSize = uDataSize;
	mstate = FileState::IN_MEMORY;

	///std::cout << pData << std::endl;
}

void DownloadFile::HttpFailure(int32_t iCode, const char* msg)
{
	FILE_LOG(logERROR) << "Download failed with code: " << iCode << "Message: " << msg;

	mstate = FileState::FAILED_DOWNLOAD;

}
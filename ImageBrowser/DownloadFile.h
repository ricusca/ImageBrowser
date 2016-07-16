#pragma once
#include "HttpCallback.h"
#include  <stdint.h>
#include "defines.h"
#include <string>

class IRequest;

class DownloadFile :public HttpCallback
{
public:
	DownloadFile();
	DownloadFile(const char* pFileName, const char* pFullUrl);	

	~DownloadFile();

	void Download();

	bool IsReadyForOpen() const;

	void GetData(char** pData, uint32_t& dataSize) const;

	const std::string& GetName() const;

	void SetFullURL(const char* pURL);

	void Free();

	// HttpCallback
public:
	virtual void HttpSuccess(const char *pData, uint32_t uDataSize) override;

	virtual void HttpFailure(int32_t iCode, const char* msg) override;

private:	
	DownloadFile(DownloadFile&);
	DownloadFile& operator=(DownloadFile file);
	void Swap(DownloadFile& first, DownloadFile& second);

private:
	std::string mFileName;
	std::string mFullUrl;
	char* mData; //could be a ref count pointer something
	unsigned int mSize;
	FileState mstate;
	IRequest* request;
};

inline bool operator<(const DownloadFile& lhs, const DownloadFile& rhs)
{
	return lhs.GetName() < rhs.GetName();
}
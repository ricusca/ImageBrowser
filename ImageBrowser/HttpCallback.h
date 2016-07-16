#pragma once

class HttpCallback
{
public:
	virtual void HttpSuccess(const char *pData, unsigned int dataSize) = 0;

	//! Called when the operation fails.
	virtual void HttpFailure(int iCode, const char* msg) = 0;

};

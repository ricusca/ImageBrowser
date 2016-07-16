#pragma once
#include "IRequest.h"
#include <string>

class HttpCallback;

class Request : public IRequest
{
public:
	explicit Request(HttpCallback* callback);

	~Request();	

	virtual bool SetUrl(const char* const url) override;

	virtual const char* const GetUrl() const override;

	virtual void SetCallback(HttpCallback* pCallback) override;

	virtual HttpCallback* GetCallback() const override;

	virtual void SetHttpMethod(HttpMethod method) override;

private:
	std::string		mUrl;
	HttpMethod		mMethod;
	HttpCallback*	mCallback;

};


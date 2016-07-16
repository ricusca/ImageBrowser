#pragma once
#include "defines.h"

class HttpCallback;

class IRequest
{
public:
	IRequest() {}

	virtual ~IRequest() {}

	virtual bool SetUrl(const char* const url) = 0;

	virtual const char* const GetUrl() const = 0;

	virtual void SetCallback(HttpCallback* pCallback) = 0;

	virtual HttpCallback* GetCallback() const = 0;

	virtual void SetHttpMethod(HttpMethod method) = 0;
};


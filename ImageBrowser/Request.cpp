#include "Request.h"
#include "HttpCallback.h"
#include <cassert>


Request::Request(HttpCallback* callback)
	:IRequest()
	, mMethod(HTTP_GET)
	, mUrl("")
	, mCallback(callback)
{
}


Request::~Request()
{
}

bool Request::SetUrl(const char* const url)
{
	assert(url != nullptr);

	bool success = true;

	if (url != nullptr && strlen(url) != 0)
	{
		mUrl = url;
	}
	else
	{
		mUrl.clear();
		success = false;
	}

	return success;
}

const char* const Request::GetUrl()const
{
	const char* url = nullptr;

	if (0 != mUrl.length())
	{
		url = mUrl.c_str();

	}

	return url;
}

void Request::SetCallback(HttpCallback* pCallback)
{
	mCallback = pCallback;
}

HttpCallback* Request::GetCallback() const
{
	return mCallback;
}

void Request::SetHttpMethod(HttpMethod method)
{
	mMethod = method;
}

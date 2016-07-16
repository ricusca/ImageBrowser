#include "HttpClient.h"

#include <sstream>
#include <string>
#include "HttpCallback.h"

const int HttpClient::NUM_THREADS = 1;

HttpClient* HttpClient::mInstance = nullptr;

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	std::string data((const char*)ptr, (size_t)size * nmemb);
	*((std::stringstream*)stream) << data << std::endl;
	return size * nmemb;
}

HttpClient::HttpClient()
	:mThreadTerminate(false)
{
	mThreads.resize(NUM_THREADS);
	for (auto& t : mThreads)
	{
		CURL* c = curl_easy_init();
		mCurls.push_back(c);
		t = std::thread([&,this] { this->Update(c); });		
	}
}

HttpClient::~HttpClient()
{
	Clean();
}

HttpClient* HttpClient::GetInstance()
{
	return mInstance;
}

HttpClient* HttpClient::Initialize()
{
	if (mInstance == nullptr)
	{
		mInstance = new HttpClient();
	}

	return mInstance;
}

void HttpClient::SubmitRequest(IRequest* request)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mRequests.push(request);

	m_cv.notify_all();
}

//download -- consumer
void HttpClient::Update(CURL* curl)
{
	while (!mThreadTerminate)
	{
		
		IRequest* request = nullptr;
		std::unique_lock<std::mutex> lock(mMutex);
		m_cv.wait(lock, [this]() {return mRequests.size() > 0; });
		request = mRequests.front();
		mRequests.pop();
		lock.unlock();
		m_cv.notify_all();

		curl_easy_setopt(curl, CURLOPT_URL, request->GetUrl());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
		std::stringstream out;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		/* Perform the request, res will get the return code */
		CURLcode res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) 
		{
			request->GetCallback()->HttpFailure(static_cast<int>(res), curl_easy_strerror(res));
		}
		else
		{
			request->GetCallback()->HttpSuccess(out.str().c_str(), out.str().length());
		}
	}
}

void HttpClient::Clean()
{
	for (auto& c : mCurls)
	{
		curl_easy_cleanup(c);
	}
	mCurls.clear();

	mThreadTerminate = true;
	for (auto& t : mThreads)
	{
		t.join();
	}		
	mCurls.clear();
}

void HttpClient::Terminate()
{
	if (mInstance)
	{	
		mInstance->Clean();
		delete mInstance;
		mInstance = nullptr;
	}
}
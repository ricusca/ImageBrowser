#include "HttpClient.h"

#include <string>
#include "HttpCallback.h"
#include "log.h"

const size_t HttpClient::NUM_THREADS = 4;
const int64_t HttpClient::MAX_IMG_SIZE = 150000;

HttpClient* HttpClient::mInstance = nullptr;

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	HttpClient::DownloadData* d = reinterpret_cast<HttpClient::DownloadData*>(stream);

	memcpy_s(d->buffer+d->offset, HttpClient::MAX_IMG_SIZE, ptr, size * nmemb);
	d->offset += size * nmemb;

	return size * nmemb;
}

HttpClient::HttpClient()
	:mThreadTerminate(false)
{
	mCurls.resize(NUM_THREADS);
	for (auto& c : mCurls)
	{
		c.curl = curl_easy_init();
		c.buffer = new char[MAX_IMG_SIZE];
		c.threadId = new std::thread([&, this] { this->Update(c); });
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

//producer -- add requests in the queue. No need for multiple producers
void HttpClient::SubmitRequest(IRequest* request)
{
	std::lock_guard<std::mutex> lock(mMutex);

	FILE_LOG(logDEBUG) << "Request for " << request->GetUrl();
	mRequests.push(request);

	m_cv.notify_all();
}

//download -- consumer
void HttpClient::Update(DownloadData& downloadData)
{
	while (!mThreadTerminate)
	{
		CURL* curl = downloadData.curl;
		downloadData.Reset();
		
		IRequest* request = nullptr;
		std::unique_lock<std::mutex> lock(mMutex);
		m_cv.wait(lock, [this]() {return mRequests.size() > 0 || mThreadTerminate; });

		if (mThreadTerminate)	break;
		
		request = mRequests.front();
		mRequests.pop();
		lock.unlock();
		m_cv.notify_all();

		curl_easy_setopt(curl, CURLOPT_URL, request->GetUrl());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &downloadData);
		//Perform the request, res will get the return code
		CURLcode res = curl_easy_perform(curl);
		// Check for errors 
		if (res != CURLE_OK) 
		{
			request->GetCallback()->HttpFailure(static_cast<int32_t>(res), curl_easy_strerror(res));
		}
		else
		{
			request->GetCallback()->HttpSuccess(downloadData.buffer, downloadData.offset);
		}
		
	}
}

void HttpClient::Clean()
{

	mThreadTerminate = true;
	for (auto& t : mCurls)
	{
		m_cv.notify_all();
		t.threadId->join();

		delete t.threadId;
		delete[] t.buffer;
		curl_easy_cleanup(t.curl);
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
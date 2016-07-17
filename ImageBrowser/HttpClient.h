#pragma once
#include <curl/curl.h>
#include <condition_variable>


#include "IRequest.h"
#include <queue>
#include <vector>
#include <thread>
#include <mutex>


class HttpClient
{

private:
	struct DownloadData
	{
		DownloadData() {offset = 0; }
		CURL* curl;
		char* buffer;
		std::thread* threadId;
		int32_t offset;

		void Reset() { offset = 0; }
	};

	friend size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

public:
	static HttpClient* GetInstance();
	static HttpClient* Initialize();
	static void Terminate();

	void SubmitRequest(IRequest* request);

protected:
	

	void Update(DownloadData& downloadData);
	void Clean();

private:


	HttpClient();
	~HttpClient();

	static const size_t NUM_THREADS;
	static const int64_t MAX_IMG_SIZE;

	static HttpClient* mInstance;
	std::queue<IRequest*> mRequests;

	std::vector<DownloadData> mCurls;
	std::mutex mMutex;
	std::condition_variable m_cv;
	volatile bool mThreadTerminate;	

};


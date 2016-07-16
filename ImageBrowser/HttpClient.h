#pragma once
#include <curl/curl.h>

#include "IRequest.h"
#include <queue>
#include <vector>
#include <thread>
#include <mutex>


class HttpClient
{

public:
	static HttpClient* GetInstance();
	static HttpClient* Initialize();
	static void Terminate();

	void SubmitRequest(IRequest* request);


protected:
	void Update(CURL* c);
	void Clean();

private:
	HttpClient();
	~HttpClient();

	static const int NUM_THREADS;

	static HttpClient* mInstance;
	std::queue<IRequest*> mRequests;

	std::vector<std::thread> mThreads;
	std::vector<CURL*> mCurls;
	std::mutex mMutex;
	std::condition_variable m_cv;
	volatile bool mThreadTerminate;	

};


#pragma once
#include <string>
#include <set>
#include "HttpClient.h"
#include "IRenderer.h"
#include "DownloadFile.h"
#include <memory>

class Board;

class Browser
{
	enum class State
	{
		STATE_GET_CONFIG,
		STATE_GET_IMAGE,
		STATE_GET_IMAGE_DONE,		
		STATE_NONE
	};
	
private:
	Browser(void) = default;
	Browser(const Browser&) = delete;
	Browser& operator=(Browser) = delete;

public:
	static Browser& GetInstance() { return mInstance; }

	void Initialize(const char* configuration);

	void Terminate();

	void Update();

	void HandleInput(int32_t x, int32_t y);

	void UpdateTitle();

private:
	void CheckConfigState();
	void CheckImagesState();
	bool ReadConfiguration();

private:
	static Browser mInstance;
	
	DownloadFile				 mConfiguration;
	std::set<DownloadFile>		 mImages;
	std::shared_ptr<IRenderer>   mRenderer;

	//static data.. maybe separate this into another class
	static TextRenderable mTitle;

	State mState;
};


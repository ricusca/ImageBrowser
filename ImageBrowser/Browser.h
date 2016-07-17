#pragma once
#include <string>
#include <unordered_map>
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

	void HandleInput(int32_t x, int32_t y, InputType inputType);

	void UpdateTitle();

	void UpdateImages();

private:
	void CheckConfigState();
	void CheckImagesState();
	bool ReadConfiguration();
	void DownloadImages();
	void DrawObject(const std::string& objectName);
	void ClearImages(int32_t startIndex, int32_t endIndex);

private:
	static Browser mInstance;
	
	DownloadFile				 mConfiguration;
	std::vector<DownloadFile>	 mImages;
	std::shared_ptr<IRenderer>   mRenderer;

	State mState;
	typedef int32_t PageNumber;
	PageNumber mActivePage;
	const static PageNumber PAGES_TO_CACHE;
	const static int32_t PAGE_SIZE;

	//maybe separate all this into another class
	static TextRenderable mTitle;
	typedef std::unordered_map< PageNumber, std::vector<ObjRenderable>> Grid; //the vector represents the objects belonging to a page
	Grid mGrid;
	ObjRenderable* mSelectedRenderable;
	
	ObjRenderable* GetRenderable(int32_t x, int32_t y);
};


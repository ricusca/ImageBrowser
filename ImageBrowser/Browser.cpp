#include "json/json.h"

#include "Browser.h"
#include "HttpClient.h"
#include "RendererFactory.h"

#include "log.h"

Browser Browser::mInstance;

TextRenderable Browser::mTitle{ "My List with pictures", 420, 50, 24 };
const Browser::PageNumber Browser::PAGES_TO_CACHE = 5;
const int32_t Browser::PAGE_SIZE = 4;

void Browser::Initialize(const char* pConfiguration)
{
	mConfiguration.SetFullURL(pConfiguration);
	HttpClient::Initialize();
	mRenderer = RendererFactory::GetInstance().CreateRenderer(RenderType::SDL_RENDERER);
	mRenderer->Initialize();

	mConfiguration.Download();
	mState = State::STATE_GET_CONFIG;

	mActivePage = 0;
	mSelectedRenderable = nullptr;
}

void Browser::Terminate()
{
	HttpClient::Terminate();
	mRenderer->Terminate();
}

void Browser::Update()
{
	switch (mState)
	{
	case Browser::State::STATE_GET_CONFIG:
		CheckConfigState();
		break;
	case Browser::State::STATE_GET_IMAGE:
		CheckImagesState();
		break;
	case Browser::State::STATE_GET_IMAGE_DONE:
		break;
	case Browser::State::STATE_NONE:
		break;
	default:
		break;
	}

	mRenderer->Update();
}

void Browser::HandleInput(int32_t x, int32_t y, InputType inputType)
{
	switch (inputType)
	{
	case InputType::MOUSE_OVER:
	{
		ObjRenderable* renderable = GetRenderable(x, y);
		if (renderable != mSelectedRenderable)
		{
			if (mSelectedRenderable)
			{
				mRenderer->Clear(*mSelectedRenderable);
				mSelectedRenderable->SetScale(1.0f);				
				mRenderer->DrawObject(*mSelectedRenderable);
			}
			mSelectedRenderable = renderable;
			
			if (mSelectedRenderable)
			{
				mSelectedRenderable->SetScale(1.11f);
				mRenderer->DrawObject(*mSelectedRenderable);
			}

			
		}
	}
		break;
	case InputType::MOUSE_CLICK:
		if (x < (Renderer::SCREEN_WIDTH - PAGE_SIZE * 171) / 2)
		{
			mActivePage--;
			if (mActivePage < 0) mActivePage = 0;
		}
		if (x > (Renderer::SCREEN_WIDTH + PAGE_SIZE * 171) / 2)
		{
			mActivePage++;
			if (static_cast<size_t>(mActivePage) > mGrid.size()-1) mActivePage = static_cast<PageNumber>(mGrid.size() - 1);
		}
		DownloadImages();
		UpdateImages();
		break;
	default:
		break;
	}
}

void Browser::CheckConfigState()
{
	if (mConfiguration.IsReadyForOpen())
	{

		ReadConfiguration();
		UpdateTitle();
		DownloadImages();
		mState = Browser::State::STATE_GET_IMAGE;
	}
}

bool Browser::ReadConfiguration()
{
	Json::Value root;
	Json::Reader reader;
	char* pData = nullptr;
	uint32_t uDataSize = 0;
	mConfiguration.GetData(&pData, uDataSize);
	bool bResult = reader.parse(pData, pData + uDataSize*sizeof(char), root);
	if (bResult == false)
	{
		FILE_LOG(logERROR) << "Failed to parse configuration data \n" << reader.getFormattedErrorMessages() << std::endl << pData;
		 
		return false;
	}

	std::string title = root.get("title", "My list").asString();
	std::string baseUrl = root.get("baseUrl", "http://ErrorReadingURL.com").asString();

	FILE_LOG(logINFO) << "Setting title: " << title << " and baseURL: " << baseUrl;
	mTitle.SetText(title);

	const Json::Value images = root["images"];
	mImages.clear();
	for (size_t idx = 0; idx < images.size(); ++idx)
	{
		std::string fullURL(baseUrl);
		std::string fileName = images[idx].asString();
		fullURL.append(images[idx].asString());
		mImages.emplace_back(fileName.c_str(),fullURL.c_str());
		FILE_LOG(logINFO) << fullURL;
	}

	//init renderables;
	size_t numPages = mImages.size() / PAGE_SIZE + (images.size() % PAGE_SIZE > 0 ? 1 : 0);
	for (size_t pageIdx = 0; pageIdx < numPages; ++pageIdx)
	{
		auto& page = mGrid[pageIdx];
		page.resize(PAGE_SIZE);
		for (size_t imageIdx = 0; imageIdx < PAGE_SIZE; ++imageIdx)
		{
			ObjRenderable& renderable = page[imageIdx];
			int32_t xPos = (Renderer::SCREEN_WIDTH - PAGE_SIZE * renderable.GetWidth())/2 + imageIdx * renderable.GetWidth();
			int32_t yPos = (Renderer::SCREEN_HEIGHT - renderable.GetHeight()) / 2;
			renderable.SetPos(xPos, yPos);
		}
	}

	return true;
}

void Browser::UpdateTitle()
{
	mRenderer->DrawTextFont(mTitle);
}

void Browser::CheckImagesState()
{
	bool downloadFinished = true;
	for (auto& f : mImages)
	{
		if (f.IsReadyForOpen() == false)
		{
			downloadFinished = false;
		}
		else
		{
			char* pData = nullptr;
			uint32_t uDataSize = 0;
			f.GetData(&pData, uDataSize);

			if (mRenderer->LoadTexture(f.GetName(), pData, uDataSize) == false)
			{
				//force download again
				f.RetryDownload();
			}
			DrawObject(f.GetName());
			//f.Free(); //optimize for memory..later
		}
	}

	if (downloadFinished)
	{
		mState = State::STATE_GET_IMAGE_DONE;
	}
}

void Browser::DownloadImages()
{
	//download left and right relative to active page
	PageNumber startIndex = std::max<PageNumber>(mActivePage - (PAGES_TO_CACHE / 2), 0, [](const PageNumber& lhs, const PageNumber& rhs) { return lhs < rhs; });//don't go beyond 0
	PageNumber endIndex = std::min<PageNumber>(mActivePage + (PAGES_TO_CACHE / 2), mGrid.size(), [](const PageNumber& lhs, const PageNumber& rhs) { return lhs > rhs; });
	for (PageNumber pageIndex = startIndex; pageIndex < endIndex; ++pageIndex)
	{
		for (size_t imageIndex = 0; imageIndex < PAGE_SIZE; ++imageIndex)
		{
			size_t fileIdx = pageIndex*PAGE_SIZE + imageIndex;
			if (fileIdx < mImages.size()) //in case we don't have PAGE_SIZE images on a pageB
			{
				DownloadFile& image = mImages.at(fileIdx);
				ObjRenderable& renderable = mGrid[pageIndex].at(imageIndex);

				if (renderable.GetName().compare(image.GetName()))
					renderable.SetName(image.GetName());

				if (image.IsReadyForOpen() == false)
					image.Download();
			}
		}
	}
}

void Browser::DrawObject(const std::string& objectName)
{
	for (auto& cell : mGrid)
	{
		//draw only left and right pages
		if (cell.first == mActivePage)
		{
			for (auto& object : cell.second)
			{
				if (object.GetName().compare(objectName) == 0)
				{
					mRenderer->DrawObject(object);
				}
			}
		}
	}
}

ObjRenderable* Browser::GetRenderable(int32_t x, int32_t y)
{
	ObjRenderable* obj = nullptr;

	auto& page = mGrid[mActivePage];
	for (auto& object : page)
	{
		if (object.HasPoint(x, y) && !object.GetName().empty())
		{
			obj = &object;
			break;
		}
	}

	return obj;
}

void Browser::UpdateImages()
{
	auto& page = mGrid[mActivePage];

	for (auto& object : page)
	{
		if (object.GetName() == "")//we have an empty object
		{ 
			mRenderer->Clear(object);
		}
		else
		{
			mRenderer->DrawObject(object);
		}
	}

}
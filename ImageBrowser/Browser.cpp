#include "Browser.h"
#include "HttpClient.h"
#include "RendererFactory.h"
#include "json/json.h"
#include <iostream>

Browser Browser::mInstance;

TextRenderable Browser::mTitle{ "My List with pictures", 310, 50, 24 };

void Browser::Initialize(const char* pConfiguration)
{
	mConfiguration.SetFullURL(pConfiguration);
	HttpClient::Initialize();
	mRenderer = RendererFactory::GetInstance().CreateRenderer(RenderType::SDL_RENDERER);
	mRenderer->Initialize();

	mConfiguration.Download();
	mState = State::STATE_GET_CONFIG;
}

void Browser::Terminate()
{

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

void Browser::HandleInput(int x, int y)
{

}

void Browser::CheckConfigState()
{
	if (mConfiguration.IsReadyForOpen())
	{

		ReadConfiguration();
		UpdateTitle();
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
		std::cout << "Failed to parse configuration data \n" << reader.getFormattedErrorMessages() << std::endl << pData << std::endl;
		return false;
	}

	std::string title = root.get("title", "My list").asString();
	std::string baseUrl = root.get("baseUrl", "http://ErrorReadingURL.com").asString();

	std::cout << title << std::endl << baseUrl << std::endl;	
	mTitle.SetText(title);

	const Json::Value images = root["images"];
	mImages.clear();
	for (size_t idx = 0; idx < images.size(); ++idx)
	{
		std::string fullURL(baseUrl);
		std::string fileName = images[idx].asString();
		fullURL.append(images[idx].asString());
		mImages.emplace(fileName.c_str(),fullURL.c_str());
		std::cout << fullURL << std::endl;
	}

	for (auto& f : mImages)
	{
		const_cast<DownloadFile&>(f).Download(); //TO DO: weird thing... check why it doesn't work without const_cast
	}

	return true;

}

void Browser::UpdateTitle()
{
	mRenderer->DrawText(mTitle);
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

			mRenderer->LoadTexture(f.GetName(), pData, uDataSize);
			mRenderer->DrawObject(ObjRenderable{ f.GetName(), 0, 0,  171, 200, 171, 207}, false);
			//f.Free();
		}
	}

	if (downloadFinished)
	{
		mState = State::STATE_GET_IMAGE_DONE;
	}
}
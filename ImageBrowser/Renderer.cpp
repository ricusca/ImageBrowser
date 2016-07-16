#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Renderer.h"

const int32_t Renderer::SCREEN_WIDTH = 800;
const int32_t Renderer::SCREEN_HEIGHT = 600;
const SDL_Color Renderer::DEFAULT_TEXT_COLOR{ 255, 10, 10 };
const SDL_Color Renderer::DEFAULT_CEAR_COLOR{ 255, 255, 255 };

Renderer::Renderer()
	:mWindow(nullptr)
	,mSDLRenderer(nullptr)
	,mFont(nullptr)
	,mTextures()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	}
}

Renderer::~Renderer()
{
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

bool Renderer::Initialize()
{
	//Initialization flag
	bool success = true;

	if (mWindow == nullptr)
	{
		//Create window
		mWindow = SDL_CreateWindow("Sony Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
	}
	if (mWindow == nullptr)
	{
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	//Create renderer
	if (mSDLRenderer == nullptr)
	{
		mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	}

	if (mSDLRenderer == nullptr)
	{
		std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	//Initialize SDL_img
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		success = false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		success = false;
	}

	if (mFont == nullptr)
	{
		SetFontType("./fonts/ANUDI.ttf", 24);
	}

	return success;
}

void Renderer::Terminate()
{
	//Destroy textures
	CleanTextures();

	//Destroy font
	TTF_CloseFont(mFont);
	mFont = nullptr;

	//Destroy window
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	//Destroy renderer
	SDL_DestroyRenderer(mSDLRenderer);
	mSDLRenderer = nullptr;
}

void Renderer::Update()
{
	//Update the surface
	SDL_RenderPresent(mSDLRenderer);
}

//Clear an area
void Renderer::Clear(int32_t x, int32_t y, int32_t width, int32_t height)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	SDL_SetRenderDrawColor(mSDLRenderer, DEFAULT_CEAR_COLOR.r, DEFAULT_CEAR_COLOR.g, DEFAULT_CEAR_COLOR.b, 255);
	//SDL_RenderFillRect(mSDLRenderer, &rect);
}

//The textures are stored in a map
void Renderer::LoadTexture(const std::string& name)
{
	if (mTextures.find(name) == mTextures.end())
	{
		SDL_Texture* texture = IMG_LoadTexture(mSDLRenderer, name.c_str());
		mTextures.emplace(name, texture);
	}
}

void Renderer::LoadTexture(const std::string& name, char* buff, int32_t size)
{	
	if (mTextures.find(name) == mTextures.end())
	{
		SDL_RWops *rw = SDL_RWFromMem(buff, size);

		SDL_Texture* texture = IMG_LoadTexture_RW(mSDLRenderer, rw, 0);

		if (texture == nullptr)
		{
			std::cerr << "Unable to load texture " << name.c_str() << " SDL Error: " << IMG_GetError() << std::endl;
			return;
		}


		mTextures.emplace(name, texture);
	}
}

void Renderer::SetTextureAlpha(const std::string& name, uint8_t alpha)
{
	auto it = mTextures.find(name);
	if (it != mTextures.end())
	{
		SDL_Texture* texture = it->second;
		if (texture != nullptr)
		{
			SDL_SetTextureAlphaMod(texture, alpha);
		}
	}
}

void Renderer::SetTextureBlendModeAlpha(const std::string& name)
{
	auto it = mTextures.find(name);
	if (it != mTextures.end())
	{
		SDL_Texture* texture = it->second;
		if (texture != nullptr)
		{
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		}
	}
}

void Renderer::SetTextureBlendModeNone(const std::string& name)
{
	auto it = mTextures.find(name);
	if (it != mTextures.end())
	{
		SDL_Texture* texture = it->second;
		if (texture != nullptr)
		{
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
		}
	}
}

void Renderer::CleanTextures()
{
	//Destroy textures
	for (auto it : mTextures)
	{
		SDL_DestroyTexture(it.second);
	}
	mTextures.clear();
}

void Renderer::DrawBackground(const std::string& name)
{
	SDL_RenderClear(mSDLRenderer);
	LoadTexture(name);

	const ObjRenderable background{ name, 0, 0, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH };
	DrawObject(background);
}

void Renderer::DrawObject(const ObjRenderable& obj, bool clear)
{
	if (clear)
	{
		this->Clear(obj.xDst, obj.yDst, obj.width, obj.height);
	}

	LoadTexture(obj.name);

	auto it = mTextures.find(obj.name);
	if (it != mTextures.end())
	{
		SDL_Texture* texture = it->second;
		if (texture == nullptr)
		{
			std::cerr << "Unable to load image " << obj.name.c_str() << " SDL Error: " << IMG_GetError() << std::endl;
			return;
		}

		SDL_Rect srcrect{ obj.xSrc, obj.ySrc, obj.width, obj.height };
		SDL_Rect dstrect{ obj.xDst, obj.yDst, obj.width, obj.height };

		SDL_RenderCopy(mSDLRenderer, texture, &srcrect, &dstrect);
	}
}

void Renderer::DrawText(const TextRenderable& txt, bool clear)
{

	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(mFont, (txt.text + txt.addText).c_str(), DEFAULT_TEXT_COLOR);

	SDL_Texture* message = SDL_CreateTextureFromSurface(mSDLRenderer, surfaceMessage);

	int w, h;
	TTF_SizeText(mFont, txt.text.c_str(), &w, &h);

	//Clear the area
	this->Clear(txt.x, txt.y, w, h);
	SDL_Rect rectMessage{ txt.x, txt.y, w, h };
	SDL_RenderCopy(mSDLRenderer, message, nullptr, &rectMessage);

	SDL_DestroyTexture(message);
	SDL_FreeSurface(surfaceMessage);
}

void Renderer::SetFontType(const std::string& name, int size)
{
	if (mFont != nullptr)
	{
		TTF_CloseFont(mFont);
		mFont = nullptr;
	}

	//Open the font
	mFont = TTF_OpenFont(name.c_str(), size);

	if (mFont == nullptr)
	{
		std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
	}
}

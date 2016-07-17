#pragma once

#include <unordered_map>
#include <SDL.h>
#include <SDL_ttf.h>
#include "IRenderable.h"
#include "IRenderer.h"

class Renderer : public IRenderer
{
public:
	Renderer();
	~Renderer();

	bool Initialize() override;
	void Terminate() override; 
	void Update() override;	
	void Clear(int32_t x, int32_t y, int32_t width, int32_t height, float scale = 1.0f) override;
	void Clear(const ObjRenderable& pRenderable) override;

	//Texture manipulation 
	void LoadTexture(const std::string& name) override;
	bool LoadTexture(const std::string& name, char* buff, int32_t size) override;
	void SetTextureAlpha(const std::string& name, uint8_t alpha) override;
	void SetTextureBlendModeAlpha(const std::string& name) override; 
	void SetTextureBlendModeNone(const std::string& name) override;
	void CleanTextures() override;

	//Object and text rendering
	void DrawBackground(const std::string& name) override;
	void DrawObject(const ObjRenderable& obj, bool clear = false) override;
	void DrawTextFont(const TextRenderable& txt) override;

	void SetFontType(const std::string&, int size) override;

	static const int32_t SCREEN_WIDTH;
	static const int32_t SCREEN_HEIGHT;

	

private:
	static const SDL_Color DEFAULT_TEXT_COLOR;
	static const SDL_Color DEFAULT_CEAR_COLOR;

	SDL_Window* mWindow;
	SDL_Renderer* mSDLRenderer;
	TTF_Font* mFont;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
};

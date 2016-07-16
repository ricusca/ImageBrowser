#pragma once

#include <cstdint>
#include <string>
#include "IRenderable.h"

class IRenderer
{
public:
	IRenderer() {}
	
	virtual ~IRenderer() {};

	virtual bool Initialize() = 0;

	virtual void Terminate() = 0;

	virtual void Update() = 0;

	virtual void Clear(int32_t x, int32_t y, int32_t width, int32_t height) = 0;

	virtual void LoadTexture(const std::string& name) = 0;
	virtual void LoadTexture(const std::string& name, char* buff, int32_t size) = 0;
	virtual void SetTextureAlpha(const std::string& name, uint8_t alpha) = 0;
	virtual void SetTextureBlendModeAlpha(const std::string& name) = 0;
	virtual void SetTextureBlendModeNone(const std::string& name) = 0;;
	virtual void CleanTextures() = 0;

	virtual void DrawBackground(const std::string& name) = 0;
	virtual void DrawObject(const ObjRenderable& obj, bool clear = false) = 0;
	virtual void DrawText(const TextRenderable& txt, bool clear = false) = 0;

	virtual void SetFontType(const std::string&, int size) = 0;
};
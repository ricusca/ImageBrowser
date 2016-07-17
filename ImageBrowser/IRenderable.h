#pragma once

#include <string>

//Only objects that are accepted by renderer
class ObjRenderable
{
	friend class Renderer;

public:
	ObjRenderable() : name(""), xSrc{ 0 }, ySrc{ 0 }, xDst{ 0 }, yDst{ 0 }, width{ 180 }, height{ 210 }, scale{ 1.0f } {};

	ObjRenderable(const std::string& pname, int32_t pxSrc, int32_t pySrc, int32_t pxDst, int32_t pyDst, int32_t pwidth, int32_t pheight) :
		name(pname), xSrc{ pxSrc }, ySrc{ pySrc }, xDst{ pxDst }, yDst{ pyDst }, width{ pwidth }, height{ pheight } {};

	const std::string& GetName() const { return name; };

	void SetName(const std::string& _name) { name = _name; }

	const int32_t GetWidth() const { return width; }

	const int32_t GetHeight() const { return height; }

	void SetPos(const int32_t x, const int32_t y) { xDst = x; yDst = y; }

	bool HasPoint(int32_t x, int32_t y) const { return  (x > xDst) && (x < (xDst + width)) && (y > yDst && y < (yDst + height)); }

	void SetScale(float s) { scale = s; }

private:
	std::string name;
	int32_t xSrc;
	int32_t ySrc;
	int32_t xDst;
	int32_t yDst;
	int32_t width;
	int32_t height;
	float scale;

};

//Object for storing text to be rendered on screen
class TextRenderable
{
	friend class Renderer;

public:
	TextRenderable() = delete;
	TextRenderable(const std::string& ptext, int32_t px, int32_t py, int32_t psize) :
		text(ptext), x{ px }, y{ py }, size{ psize } {};

	void SetText(const std::string& ptext) { text = ptext; }
	void SetAddText(const std::string& paddtext) { addText = paddtext; }

private:
	std::string text;
	std::string addText;
	int32_t x;
	int32_t y;
	int32_t	 size;
};

#pragma once

#include <string>

//Only objects that are accepted by renderer
class ObjRenderable
{
	friend class Renderer;

public:
	ObjRenderable() : name(""), xSrc{ 0 }, ySrc{ 0 }, xDst{ 0 }, yDst{ 0 }, width{ 0 }, height{ 0 } {};

	ObjRenderable(const std::string& pname, int pxSrc, int pySrc, int pxDst, int pyDst, int pwidth, int pheight) :
		name(pname), xSrc{ pxSrc }, ySrc{ pySrc }, xDst{ pxDst }, yDst{ pyDst }, width{ pwidth }, height{ pheight } {};

	const std::string& GetName() const { return name; };

private:
	std::string name;
	int xSrc;
	int ySrc;
	int xDst;
	int yDst;
	int width;
	int height;

};

//Object for storing text to be rendered on screen
class TextRenderable
{
	friend class Renderer;

public:
	TextRenderable() = delete;
	TextRenderable(const std::string& ptext, int px, int py, int psize) :
		text(ptext), x{ px }, y{ py }, size{ psize } {};

	void SetText(const std::string& ptext) { text = ptext; }
	void SetAddText(const std::string& paddtext) { addText = paddtext; }

private:
	std::string text;
	std::string addText;
	int x;
	int y;
	int size;
};

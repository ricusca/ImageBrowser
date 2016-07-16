#pragma once
#include <cstdint>

class IRenderer;

class Cell
{
public:
	explicit Cell(IRenderer* renderer);
	~Cell();

	//getters
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetRow() const { return y / mWidth; }
	int GetCol() const { return x / mHeight; }
	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }
	void SetWidth(int pwidth) { mWidth = pwidth; }
	void SetHeight(int pheight) { mHeight = pheight; }

	void Render() const;

private:
	int32_t mWidth;
	int32_t mHeight;
	int32_t x;
	int32_t y;
	
	IRenderer* mRenderer;
};


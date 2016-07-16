#include "Cell.h"

#include "Browser.h"
#include "Renderer.h"

Cell::Cell(IRenderer* renderer)
	:mRenderer(renderer)
{
}


Cell::~Cell()
{
}

void Cell::Render() const
{
	mRenderer->DrawObject(ObjRenderable{ "test", 0, 0, x, y, mWidth, mHeight }, true);
}
#pragma once

#include "Renderer.h"
#include <memory>
#include <map>

enum class RenderType
{
	SDL_RENDERER,
	SDL_NONE
};

class RendererFactory
{

public:
	static RendererFactory& GetInstance() { return mInstance; }

	std::shared_ptr<IRenderer> CreateRenderer(RenderType renderType)
	{
		auto renndererIt = mRenderers.find(renderType);
		if (renndererIt == mRenderers.end())
		{
			switch (renderType)
			{
			case RenderType::SDL_RENDERER:
				mRenderers[renderType] = std::make_shared<Renderer>();
				break;
			case RenderType::SDL_NONE:
				break;
			default:
				break;
			}
		}

		return mRenderers[renderType];
	}

private:
	std::map<RenderType, std::shared_ptr<IRenderer>> mRenderers;
	static RendererFactory mInstance;
};

RendererFactory RendererFactory::mInstance;
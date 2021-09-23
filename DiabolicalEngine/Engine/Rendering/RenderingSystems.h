#pragma once
#include "..\ECS\flecs.h"
#include "RendererComponents.h"
namespace BasicRendererSystem
{
	bool Initialize(BasicRenderer& renderer);
	void Render(BasicRenderer& renderer);
};


namespace SpriteRenderer {};


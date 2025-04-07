#pragma once

#include "engine/rendering/assets/cLight.h"
#include "iAssetManager.h"

namespace df
{
	class cLightManager final : public iAssetManager< cLightManager, cLight >
	{
	public:
		DF_DisableCopyAndMove( cLightManager )

		cLightManager();
		~cLightManager() override = default;
	};
}

#pragma once

#include "engine/rendering/assets/iQuad.h"
#include "iAssetManager.h"

namespace df
{
	class cQuadManager final : public iAssetManager< cQuadManager, iQuad >
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cQuadManager )

		cQuadManager();
		~cQuadManager() override = default;
	};
}

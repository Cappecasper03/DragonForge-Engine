#pragma once

#include "engine/graphics/cameras/cCamera.h"
#include "iAssetManager.h"

namespace df
{
	class cCameraManager final : public iAssetManager< cCameraManager, cCamera >
	{
	public:
		DF_DisableCopyAndMove( cCameraManager );

		cCameraManager();
		~cCameraManager() override = default;

		cCamera* current;
	};
}

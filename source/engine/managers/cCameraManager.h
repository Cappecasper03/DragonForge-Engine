#pragma once

#include "engine/graphics/cameras/cCamera.h"
#include "iObjectManager.h"

namespace df
{
	class cCameraManager final : public iObjectManager< cCameraManager, cCamera >
	{
	public:
		DF_DeleteCopyAndMove( cCameraManager );

		cCameraManager();
		~cCameraManager() override = default;

		cCamera* m_current;
	};
}

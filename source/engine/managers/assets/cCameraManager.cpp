#include "cCameraManager.h"

#include "engine/core/Log.h"
#include "engine/graphics/cameras/cCamera.h"
#include "profiling/ProfilingMacros.h"

namespace df
{
	cCameraManager::cCameraManager()
	{
		DF_ProfilingScopeCpu;

		current = create( "default_3d", cCamera::eType::ePerspective, cColor( .5f, .75f, 1, 1 ), 90.f );
		create( "default_2d", cCamera::eType::eOrthographic, color::white, 90.f, -1.f, 100.f );
	}
}

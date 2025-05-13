#include "cCameraManager.h"

#include "engine/core/Log.h"
#include "engine/graphics/cameras/cCamera.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cCameraManager::cCameraManager()
	{
		DF_ProfilingScopeCpu;

		m_current = create( "default_3d", cCamera::eType::kPerspective, cColor( .5f, .75f, 1, 1 ), 90.f );
		create( "default_2d", cCamera::eType::kOrthographic, color::white, 90.f, -1.f, 100.f );
	}
}

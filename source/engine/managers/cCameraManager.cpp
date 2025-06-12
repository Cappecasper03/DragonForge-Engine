#include "cCameraManager.h"

#include "engine/graphics/cameras/cCamera.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cCameraManager::cCameraManager()
	{
		DF_ProfilingScopeCpu;

		cCamera::sDescription description{
			.name        = "default_3d",
			.type        = cCamera::eType::kPerspective,
			.clear_color = cColor( .5f, .75f, 1, 1 ),
			.fov         = 90,
		};
		m_current = new cCamera( description );
		add( m_current );

		description = {
			.name        = "default_2d",
			.type        = cCamera::eType::kOrthographic,
			.clear_color = color::white,
			.fov         = 90,
			.near_clip   = -1.f,
			.far_clip    = 100.f,
		};
		add( new cCamera( description ) );
	}
}

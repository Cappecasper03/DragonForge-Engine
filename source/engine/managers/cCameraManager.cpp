#include "cCameraManager.h"

#include <ranges>

#include "engine/graphics/cameras/cCamera.h"
#include "engine/graphics/cameras/cRenderTextureCamera2D.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cCameraManager::cCameraManager()
		: m_current( nullptr )
		, m_camera_main( nullptr )
		, m_camera_gui( nullptr )
	{
		DF_ProfilingScopeCpu;

		cCamera::sDescription description{
			.name        = "default_3d",
			.type        = cCamera::eType::kPerspective,
			.clear_color = cColor( .5f, .75f, 1, 1 ),
			.fov         = 90,
			.near_clip   = .1f,
			.far_clip    = 10000,
		};
		m_camera_main = new cCamera( description );

		description = {
			.name        = "default_2d",
			.type        = cCamera::eType::kOrthographic,
			.clear_color = color::white,
			.fov         = 90,
			.near_clip   = -1,
			.far_clip    = 100,
		};
		m_camera_gui = new cCamera( description );
	}

	cCameraManager::~cCameraManager()
	{
		DF_ProfilingScopeCpu;

		delete m_camera_gui;
		delete m_camera_main;

		for( const cRenderTextureCamera2D* camera: m_texture_cameras | std::views::values )
			delete camera;
	}
}

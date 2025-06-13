#include "cCameraManager.h"

#include "engine/graphics/cameras/cCamera.h"
#include "engine/graphics/cameras/cRenderTextureCamera2D.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cCameraManager::cCameraManager()
		: m_current( nullptr )
		, m_current_is_regular( false )
		, m_deferred_camera( nullptr )
		, m_camera_main( nullptr )
		, m_camera_gui( nullptr )
	{}

	cCameraManager::~cCameraManager()
	{
		DF_ProfilingScopeCpu;

		delete m_camera_gui;
		delete m_camera_main;
		delete m_deferred_camera;
	}
}

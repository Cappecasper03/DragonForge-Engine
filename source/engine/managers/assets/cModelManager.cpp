﻿#include "cModelManager.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/cModel_opengl.h"
#include "engine/graphics/vulkan/assets/cModel_vulkan.h"

namespace df
{
	cModelManager::cModelManager()
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl:
			{
				m_default_render_callback = opengl::cModel_opengl::createDefaults();
				break;
			}
			case cRenderer::kVulkan:
			{
				m_default_render_callback = vulkan::cModel_vulkan::createDefaults();
				break;
			}
		}
	}

	cModelManager::~cModelManager()
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl:
				break;
			case cRenderer::kVulkan:
			{
				vulkan::cModel_vulkan::destroyDefaults();
				break;
			}
		}
	}

	iModel* cModelManager::load( const std::string& _name, const std::string& _file_path, const unsigned _load_flags )
	{
		DF_ProfilingScopeCpu;

		iModel* model = nullptr;
		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl:
			{
				model = create< opengl::cModel_opengl >( _name );
				break;
			}
			case cRenderer::kVulkan:
			{
				model = create< vulkan::cModel_vulkan >( _name );
				break;
			}
		}

		if( model )
			model->load( _file_path, _load_flags );

		return model;
	}
}

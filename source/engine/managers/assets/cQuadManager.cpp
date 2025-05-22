#include "cQuadManager.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/cQuad_opengl.h"
#include "engine/graphics/vulkan/assets/cQuad_vulkan.h"

namespace df
{
	cQuadManager::cQuadManager()
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl:
			{
				m_default_render_callback = opengl::cQuad_opengl::createDefaults();
				break;
			}
			case cRenderer::kVulkan:
			{
				m_default_render_callback = vulkan::cQuad_vulkan::createDefaults();
				break;
			}
		}
	}

	cQuadManager::~cQuadManager()
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl:
				break;
			case cRenderer::kVulkan:
			{
				vulkan::cQuad_vulkan::destroyDefaults();
				break;
			}
		}
	}

	iQuad* cQuadManager::load( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl:
				return create< opengl::cQuad_opengl >( _name, _position, _size, _color );
			case cRenderer::kVulkan:
				return create< vulkan::cQuad_vulkan >( _name, _position, _size, _color );
		}

		return nullptr;
	}
}

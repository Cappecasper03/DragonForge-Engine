#include "cQuadManager.h"

#include "../cRenderCallbackManager.h"
#include "engine/rendering/OpenGL/callbacks/DefaultQuadCB.h"
#include "engine/rendering/vulkan/assets/cQuad.h"
#include "engine/rendering/Vulkan/callbacks/DefaultQuadCB.h"

namespace df
{
	cQuadManager::cQuadManager()
	{
		ZoneScoped;

		switch( cRendererSingleton::getInstanceType() )
		{
			case cRendererSingleton::kOpenGL:
			{
				m_default_render_callback = cRenderCallbackManager::create( "default_quad", opengl::render_callback::defaultQuad );
			}
			break;
			case cRendererSingleton::kVulkan:
			{ /*m_default_render_callback = cRenderCallbackManager::create( "default_quad", vulkan::render_callback::defaultQuad );*/
			}
			break;
		}
	}

	iQuad* cQuadManager::create( const std::string& _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
	{
		switch( cRendererSingleton::getInstanceType() )
		{
			case cRendererSingleton::kOpenGL:
				return iAssetManager::create< opengl::cQuad >( _name, _position, _size, _color );
			case cRendererSingleton::kVulkan:
				return iAssetManager::create< vulkan::cQuad >( _name, _position, _size, _color );
		}

		return nullptr;
	}
}

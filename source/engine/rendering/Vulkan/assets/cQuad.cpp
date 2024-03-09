#include "cQuad.h"

#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df::vulkan
{
	cQuad::cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

		cRenderer::createBuffer( sizeof( sVertex ) * 4,
		                         VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		                         VMA_MEMORY_USAGE_GPU_ONLY,
		                         this,
		                         renderer->memory_allocator );
	}

	void cQuad::render()
	{
		ZoneScoped;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cPipeline >( cQuadManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cPipeline >( render_callback, this );
		else
			cRenderCallbackManager::render< cPipeline >( cQuadManager::getDefaultRenderCallback(), this );
	}
}

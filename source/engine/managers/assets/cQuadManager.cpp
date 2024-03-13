#include "cQuadManager.h"

#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/opengl/callbacks/DefaultQuadCB_opengl.h"
#include "engine/rendering/vulkan/assets/cQuad_vulkan.h"
#include "engine/rendering/vulkan/callbacks/DefaultQuadCB_vulkan.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/misc/Helper_vulkan.h"

namespace df
{
	cQuadManager::cQuadManager()
	{
		ZoneScoped;

		switch( cRenderer::getInstanceType() )
		{
			case cRenderer::kOpenGL:
				m_default_render_callback = cRenderCallbackManager::create( "default_quad", opengl::render_callback::defaultQuad );
				break;
			case cRenderer::kVulkan:
				createVulkanDefault();
				break;
		}
	}

	iQuad* cQuadManager::load( const std::string& _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
	{
		switch( cRenderer::getInstanceType() )
		{
			case cRenderer::kOpenGL:
				return create< opengl::cQuad_opengl >( _name, _position, _size, _color );
			case cRenderer::kVulkan:
				return create< vulkan::cQuad_vulkan >( _name, _position, _size, _color );
		}

		return nullptr;
	}

	void cQuadManager::createVulkanDefault()
	{
		const vulkan::cRenderer_vulkan* renderer = reinterpret_cast< vulkan::cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		vulkan::cPipeline_vulkan::sCreateInfo pipeline_create_info{
			.logical_device = renderer->logical_device,
		};

		constexpr VkPushConstantRange buffer_range{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.offset     = 0,
			.size       = sizeof( vulkan::cQuad_vulkan::sVertexConstants ),
		};
		pipeline_create_info.push_constant_ranges.push_back( buffer_range );

		pipeline_create_info.setShaders( vulkan::helper::util::createShaderModule( "default_quad_vertex", renderer->logical_device ),
		                                 vulkan::helper::util::createShaderModule( "default_quad_fragment", renderer->logical_device ) );
		pipeline_create_info.setInputTopology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST );
		pipeline_create_info.setpolygonMode( VK_POLYGON_MODE_FILL );
		pipeline_create_info.setCullMode( VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE );
		pipeline_create_info.setDepthFormat( VK_FORMAT_UNDEFINED );
		pipeline_create_info.setMultisamplingNone();

		m_default_render_callback = cRenderCallbackManager::create( "default_quad", pipeline_create_info, vulkan::render_callback::defaultQuad );
	}
}

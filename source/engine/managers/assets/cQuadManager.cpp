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

		vulkan::sPipelineCreateInfo pipeline_create_info{
			.logical_device = renderer->logical_device,
		};

		pipeline_create_info.descriptor_layouts.push_back( renderer->vertex_scene_constants_layout );

		constexpr VkPushConstantRange vertex_buffer_range{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.offset     = 0,
			.size       = sizeof( vulkan::cQuad_vulkan::sVertexConstants ),
		};
		pipeline_create_info.push_constant_ranges.push_back( vertex_buffer_range );

		pipeline_create_info.setShaders( vulkan::helper::util::createShaderModule( "default_quad_vertex" ), vulkan::helper::util::createShaderModule( "default_quad_fragment" ) );
		pipeline_create_info.setInputTopology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST );
		pipeline_create_info.setpolygonMode( VK_POLYGON_MODE_FILL );
		pipeline_create_info.setCullMode( VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE );
		pipeline_create_info.setDepthFormat( VK_FORMAT_UNDEFINED );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthtest( true, VK_COMPARE_OP_GREATER_OR_EQUAL );
		pipeline_create_info.setColorFormat( renderer->getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );
		pipeline_create_info.enableBlendingAdditive();

		m_default_render_callback = cRenderCallbackManager::create( "default_quad", pipeline_create_info, vulkan::render_callback::defaultQuad );
	}
}

#include "cQuadManager.h"

#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/opengl/callbacks/DefaultQuadCB_opengl.h"
#include "engine/rendering/vulkan/assets/cQuad_vulkan.h"
#include "engine/rendering/vulkan/callbacks/DefaultQuadCB_vulkan.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/descriptor/sDescriptorLayoutBuilder_vulkan.h"
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

	cQuadManager::~cQuadManager()
	{
		ZoneScoped;
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

		vulkan::sPipelineCreateInfo pipeline_create_info{};

		constexpr VkVertexInputBindingDescription binding_description{
			.binding   = 0,
			.stride    = sizeof( iQuad::sVertex ),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
		};
		pipeline_create_info.vertex_input_binding.push_back( binding_description );

		VkVertexInputAttributeDescription attribute_descriptions{
			.location = 0,
			.binding  = 0,
			.format   = VK_FORMAT_R32G32B32_SFLOAT,
			.offset   = offsetof( iQuad::sVertex, iQuad::sVertex::position ),
		};
		pipeline_create_info.vertex_input_attribute.push_back( attribute_descriptions );

		attribute_descriptions = {
			.location = 1,
			.binding  = 0,
			.format   = VK_FORMAT_R32G32_SFLOAT,
			.offset   = offsetof( iQuad::sVertex, iQuad::sVertex::tex_coord ),
		};
		pipeline_create_info.vertex_input_attribute.push_back( attribute_descriptions );

		constexpr VkPushConstantRange push_constant_range{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			.offset     = 0,
			.size       = sizeof( vulkan::cQuad_vulkan::sPushConstants ),
		};
		pipeline_create_info.push_constant_ranges.push_back( push_constant_range );

		pipeline_create_info.descriptor_layouts.push_back( renderer->vertex_scene_uniform_layout );

		pipeline_create_info.setShaders( vulkan::helper::util::createShaderModule( "default_quad_vertex" ), vulkan::helper::util::createShaderModule( "default_quad_fragment" ) );
		pipeline_create_info.setInputTopology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST );
		pipeline_create_info.setpolygonMode( VK_POLYGON_MODE_FILL );
		pipeline_create_info.setCullMode( VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE );
		pipeline_create_info.setColorFormat( renderer->getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.disableDepthtest();
		pipeline_create_info.disableBlending();

		m_default_render_callback = cRenderCallbackManager::create( "default_quad", pipeline_create_info, vulkan::render_callback::defaultQuad );
	}
}

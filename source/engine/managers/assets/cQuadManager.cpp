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

	iQuad* cQuadManager::load( const std::string& _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
	{
		ZoneScoped;

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
		ZoneScoped;

		const vulkan::cRenderer_vulkan* renderer = reinterpret_cast< vulkan::cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		vulkan::sPipelineCreateInfo_vulkan pipeline_create_info{};

		pipeline_create_info.vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( iQuad::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.vertex_input_attribute.emplace_back( 0,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iQuad::sVertex, iQuad::sVertex::position ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 1,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iQuad::sVertex, iQuad::sVertex::tex_coord ) ) );

		pipeline_create_info.push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
		                                                        0,
		                                                        static_cast< uint32_t >( sizeof( vulkan::cQuad_vulkan::sPushConstants ) ) );

		vulkan::sDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eCombinedImageSampler );
		vulkan::cQuad_vulkan::texture_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.descriptor_layouts.push_back( renderer->getVertexSceneUniformLayout().get() );
		pipeline_create_info.descriptor_layouts.push_back( vulkan::cQuad_vulkan::texture_layout );

		pipeline_create_info.setShaders( vulkan::helper::util::createShaderModule( "default_quad_vertex" ), vulkan::helper::util::createShaderModule( "default_quad_fragment" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setpolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormat( renderer->getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.disableDepthtest();
		pipeline_create_info.disableBlending();

		m_default_render_callback = cRenderCallbackManager::create( "default_quad", pipeline_create_info, vulkan::render_callback::defaultQuad );
	}
}

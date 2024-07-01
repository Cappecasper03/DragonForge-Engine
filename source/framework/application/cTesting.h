#pragma once

#include "cApplication.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/rendering/assets/cameras/cFreeFlightCamera.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"
#include "engine/rendering/OpenGL/assets/cQuad_opengl.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "engine/rendering/vulkan/misc/Helper_vulkan.h"
#include "engine/rendering/vulkan/pipeline/cPipeline_vulkan.h"
#include "engine/rendering/vulkan/pipeline/sPipelineCreateInfo.h"
#include "imgui.h"

struct vertex
{
	glm::vec4 color;
	glm::vec3 position;
};

struct constants
{
	glm::mat4 matrix;
};

class cTesting
{
public:
	cTesting();
	~cTesting();

	void render();
	void imgui();
	void input( const df::input::sInput& _input );

	df::cFreeFlightCamera*        camera;
	df::vulkan::cPipeline_vulkan* pipeline;

	std::vector< vertex >   vertices;
	std::vector< unsigned > indices;

	df::vulkan::sAllocatedBuffer vertex_buffer;
	df::vulkan::sAllocatedBuffer index_buffer;

	df::vulkan::sAllocatedBuffer uniform_buffer;
	VkDescriptorSetLayout        uniform_layout;
};

inline cTesting::cTesting()
{
	/*auto quad = */ df::cQuadManager::load( "quad", glm::vec3( 0, 0, 0 ), glm::vec2( 6, 4 ), df::color::blue );
	// quad->loadTexture( "data/resources/window.png", true );
	// df::cModelManager::load( "backpack", "data/models/survival-guitar-backpack" );

	camera = new df::cFreeFlightCamera( "freeflight", 1, .1f );
	camera->setActive( true );

	df::cEventManager::subscribe( df::event::update, camera, &df::cFreeFlightCamera::update );
	df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render );
	df::cEventManager::subscribe( df::event::imgui, this, &cTesting::imgui );
	df::cEventManager::subscribe( df::event::input, this, &cTesting::input );

	df::cRenderer::getRenderInstance()->setCursorInputMode( GLFW_CURSOR_DISABLED );

	{
		{
			vertices.resize( 4 );
			indices.resize( 6 );

			vertices[ 0 ].position = { 0.5, -0.5, 0 };
			vertices[ 1 ].position = { 0.5, 0.5, 0 };
			vertices[ 2 ].position = { -0.5, -0.5, 0 };
			vertices[ 3 ].position = { -0.5, 0.5, 0 };

			vertices[ 0 ].color = { 0, 0, 0, 1 };
			vertices[ 1 ].color = { 0, 0, 1, 1 };
			vertices[ 2 ].color = { 1, 0, 0, 1 };
			vertices[ 3 ].color = { 0, 1, 0, 1 };

			indices[ 0 ] = 0;
			indices[ 1 ] = 1;
			indices[ 2 ] = 2;

			indices[ 3 ] = 2;
			indices[ 4 ] = 1;
			indices[ 5 ] = 3;
		}

		const df::vulkan::cRenderer_vulkan* renderer = reinterpret_cast< df::vulkan::cRenderer_vulkan* >( df::cRenderer::getRenderInstance() );

		{
			const size_t vertex_buffer_size = sizeof( *vertices.data() ) * vertices.size();
			const size_t index_buffer_size  = sizeof( *indices.data() ) * indices.size();

			vertex_buffer = df::vulkan::helper::util::createBuffer( vertex_buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY );

			index_buffer = df::vulkan::helper::util::createBuffer( index_buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY );

			const df::vulkan::sAllocatedBuffer staging_buffer
				= df::vulkan::helper::util::createBuffer( vertex_buffer_size + index_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY );

			void* data;
			vmaMapMemory( renderer->memory_allocator, staging_buffer.allocation, &data );
			std::memcpy( data, vertices.data(), vertex_buffer_size );
			std::memcpy( static_cast< char* >( data ) + vertex_buffer_size, indices.data(), index_buffer_size );
			vmaUnmapMemory( renderer->memory_allocator, staging_buffer.allocation );

			renderer->immediateSubmit(
				[ & ]( const VkCommandBuffer _buffer )
				{
					const VkBufferCopy vertex_copy{
						.srcOffset = 0,
						.dstOffset = 0,
						.size      = vertex_buffer_size,
					};

					vkCmdCopyBuffer( _buffer, staging_buffer.buffer, vertex_buffer.buffer, 1, &vertex_copy );

					const VkBufferCopy index_copy{
						.srcOffset = vertex_buffer_size,
						.dstOffset = 0,
						.size      = index_buffer_size,
					};

					vkCmdCopyBuffer( _buffer, staging_buffer.buffer, index_buffer.buffer, 1, &index_copy );
				} );

			df::vulkan::helper::util::destroyBuffer( staging_buffer );

			uniform_buffer = df::vulkan::helper::util::createBuffer( sizeof( constants ), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU );
		}

		{
			df::vulkan::sPipelineCreateInfo pipeline_create_info{};

			constexpr VkVertexInputBindingDescription binding_description{
				.binding   = 0,
				.stride    = sizeof( vertex ),
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
			};
			pipeline_create_info.vertex_input_binding.push_back( binding_description );

			VkVertexInputAttributeDescription attribute_descriptions{
				.location = 0,
				.binding  = 0,
				.format   = VK_FORMAT_R32G32B32A32_SFLOAT,
				.offset   = offsetof( vertex, vertex::color ),
			};
			pipeline_create_info.vertex_input_attribute.push_back( attribute_descriptions );

			attribute_descriptions = {
				.location = 1,
				.binding  = 0,
				.format   = VK_FORMAT_R32G32B32_SFLOAT,
				.offset   = offsetof( vertex, vertex::position ),
			};
			pipeline_create_info.vertex_input_attribute.push_back( attribute_descriptions );

			df::vulkan::sDescriptorLayoutBuilder_vulkan layout_builder{};
			layout_builder.addBinding( 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );

			uniform_layout = layout_builder.build( VK_SHADER_STAGE_VERTEX_BIT );
			pipeline_create_info.descriptor_layouts.push_back( uniform_layout );

			pipeline_create_info.setShaders( df::vulkan::helper::util::createShaderModule( "vert" ), df::vulkan::helper::util::createShaderModule( "frag" ) );
			pipeline_create_info.setInputTopology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST );
			pipeline_create_info.setpolygonMode( VK_POLYGON_MODE_FILL );
			pipeline_create_info.setCullMode( VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE );
			pipeline_create_info.setMultisamplingNone();
			pipeline_create_info.disableDepthtest();
			pipeline_create_info.disableBlending();
			pipeline_create_info.setColorFormat( renderer->getRenderColorFormat() );
			pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );

			pipeline = new df::vulkan::cPipeline_vulkan( pipeline_create_info );
		}
	}
}

inline cTesting::~cTesting()
{
	df::cEventManager::unsubscribe( df::event::input, this );
	df::cEventManager::unsubscribe( df::event::imgui, this );
	df::cEventManager::unsubscribe( df::event::render_3d, this );
	df::cEventManager::unsubscribe( df::event::update, camera );
}

inline void cTesting::render()
{
	camera->beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// df::cQuadManager::render();
	// df::cModelManager::render();

	df::vulkan::cRenderer_vulkan* renderer       = reinterpret_cast< df::vulkan::cRenderer_vulkan* >( df::cRenderer::getRenderInstance() );
	const VkCommandBuffer         command_buffer = renderer->getCurrentFrame().command_buffer;

	const constants constant{
		.matrix = camera->view_projection,
	};

	void* data;
	vmaMapMemory( renderer->memory_allocator, uniform_buffer.allocation, &data );
	std::memcpy( data, &constant, sizeof( constants ) );
	vmaUnmapMemory( renderer->memory_allocator, uniform_buffer.allocation );

	std::vector< VkDescriptorSet > descriptor_sets;
	descriptor_sets.push_back( renderer->getCurrentFrame().descriptors.allocate( uniform_layout ) );

	df::vulkan::sDescriptorWriter_vulkan writer_scene;
	writer_scene.writeBuffer( 0, uniform_buffer.buffer, sizeof( constants ), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );
	writer_scene.updateSet( descriptor_sets.back() );

	vkCmdBindPipeline( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline );

	vkCmdBindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, static_cast< uint32_t >( descriptor_sets.size() ), descriptor_sets.data(), 0, nullptr );

	renderer->setViewportScissor();

	const VkBuffer         vertex_buffers[] = { vertex_buffer.buffer };
	constexpr VkDeviceSize offsets[]        = { 0 };
	vkCmdBindVertexBuffers( command_buffer, 0, 1, vertex_buffers, offsets );

	vkCmdBindIndexBuffer( command_buffer, index_buffer.buffer, 0, VK_INDEX_TYPE_UINT32 );

	vkCmdDrawIndexed( command_buffer, static_cast< uint32_t >( indices.size() ), 1, 0, 0, 0 );

	camera->endRender();
}

inline void cTesting::imgui()
{}

inline void cTesting::input( const df::input::sInput& /*_input*/ )
{
	if( df::cInputManager::checkKey( GLFW_KEY_ESCAPE ) == df::input::kPress )
		cApplication::quit();
}

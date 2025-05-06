#include "cModel_vulkan.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <ranges>

#include "cMesh_vulkan.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "graphics/cRenderer.h"
#include "graphics/vulkan/callbacks/cDefaultMesh_vulkan.h"
#include "graphics/vulkan/cRenderer_vulkan.h"
#include "graphics/vulkan/descriptor/sDescriptorLayoutBuilder_vulkan.h"
#include "graphics/vulkan/types/Helper_vulkan.h"

namespace df::vulkan
{

	cModel_vulkan::cModel_vulkan( std::string _name )
		: iModel( std::move( _name ) )
	{
		DF_ProfilingScopeCpu;
	}

	iRenderCallback* cModel_vulkan::createDefaults()
	{
		DF_ProfilingScopeCpu;

		if( cRenderer::isDeferred() )
			return createDefaultsDeferred();

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		sPipelineCreateInfo_vulkan pipeline_create_info{ .name = "forward_mesh_ambient" };

		pipeline_create_info.vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( iMesh::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.vertex_input_attribute.emplace_back( 0,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::position ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 1,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::normal ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 2,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tangent ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 3,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::bitangent ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 4,
		                                                          0,
		                                                          vk::Format::eR32G32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tex_coords ) ) );

		pipeline_create_info.push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex, 0, static_cast< uint32_t >( sizeof( cMesh_vulkan::sPushConstants ) ) );

		sDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampler );
		cMesh_vulkan::s_descriptor_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.descriptor_layouts.push_back( sFrameData_vulkan::s_vertex_scene_descriptor_set_layout.get() );
		pipeline_create_info.descriptor_layouts.push_back( cMesh_vulkan::s_descriptor_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "forward_mesh_ambient.vert" ), helper::util::createShaderModule( "forward_mesh_ambient.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormat( renderer->getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthTest( true, vk::CompareOp::eLessOrEqual );
		pipeline_create_info.disableBlending();

		return cRenderCallbackManager::create( "forward_mesh", pipeline_create_info, render_callbacks::cDefaultMesh_vulkan::forwardMesh );
	}

	void cModel_vulkan::destroyDefaults()
	{
		DF_ProfilingScopeCpu;

		cMesh_vulkan::s_descriptor_layout.reset();
	}

	bool cModel_vulkan::processNode( const aiNode* _node, const aiScene* _scene )
	{
		DF_ProfilingScopeCpu;

		if( !_node )
			return false;

		for( unsigned i = 0; i < _node->mNumMeshes; ++i )
			meshes.push_back( new cMesh_vulkan( _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

		for( unsigned i = 0; i < _node->mNumChildren; ++i )
			processNode( _node->mChildren[ i ], _scene );

		return true;
	}

	iRenderCallback* cModel_vulkan::createDefaultsDeferred()
	{
		DF_ProfilingScopeCpu;

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		sPipelineCreateInfo_vulkan pipeline_create_info{ .name = "deferred_mesh" };

		pipeline_create_info.vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( iMesh::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.vertex_input_attribute.emplace_back( 0,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::position ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 1,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::normal ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 2,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tangent ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 3,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::bitangent ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 4,
		                                                          0,
		                                                          vk::Format::eR32G32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tex_coords ) ) );

		pipeline_create_info.push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex, 0, static_cast< uint32_t >( sizeof( cMesh_vulkan::sPushConstants ) ) );

		sDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eUniformBuffer );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 2, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 3, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 4, vk::DescriptorType::eSampler );
		cMesh_vulkan::s_descriptor_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.descriptor_layouts.push_back( cMesh_vulkan::s_descriptor_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "deferred_mesh.vert" ), helper::util::createShaderModule( "deferred_mesh.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormats( std::vector( 3, renderer->getRenderColorFormat() ) );
		pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthTest( true, vk::CompareOp::eLessOrEqual );
		pipeline_create_info.disableBlending();

		return cRenderCallbackManager::create( "deferred_mesh", pipeline_create_info, render_callbacks::cDefaultMesh_vulkan::deferredMesh );
	}
}

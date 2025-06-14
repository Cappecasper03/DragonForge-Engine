#include "cModel_vulkan.h"

#include <assimp/scene.h>
#include <ranges>

#include "cMesh_vulkan.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/callbacks/cDefaultMesh_vulkan.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorLayoutBuilder_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{

	cModel_vulkan::cModel_vulkan( const std::string& _name )
		: iModel( _name )
	{
		DF_ProfilingScopeCpu;
	}

	iRenderCallback* cModel_vulkan::createDefaults()
	{
		DF_ProfilingScopeCpu;

		if( cRenderer::isDeferred() )
			return createDefaultsDeferred();

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		cPipelineCreateInfo_vulkan pipeline_create_info{ .m_name = "forward_mesh" };

		pipeline_create_info.m_vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( iMesh::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.m_vertex_input_attribute.emplace_back( 0,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::position ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 1,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::normal ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 2,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tangent ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 3,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::bitangent ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 4,
		                                                            0,
		                                                            vk::Format::eR32G32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tex_coords ) ) );

		pipeline_create_info.m_push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
		                                                          0,
		                                                          static_cast< uint32_t >( sizeof( cMesh_vulkan::sPushConstants ) ) );

		cDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eSampler );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 2, vk::DescriptorType::eSampledImage );
		cMesh_vulkan::s_descriptor_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.m_descriptor_layouts.push_back( sFrameData_vulkan::s_vertex_scene_descriptor_set_layout.get() );
		pipeline_create_info.m_descriptor_layouts.push_back( cMesh_vulkan::s_descriptor_layout.get() );
		pipeline_create_info.m_descriptor_layouts.push_back( sFrameData_vulkan::s_fragment_scene_descriptor_set_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "forward_mesh.vert" ), helper::util::createShaderModule( "forward_mesh.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormat( graphics_api->getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( graphics_api->getRenderDepthFormat() );
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
			m_meshes.push_back( MakeUnique< cMesh_vulkan >( _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

		for( unsigned i = 0; i < _node->mNumChildren; ++i )
			processNode( _node->mChildren[ i ], _scene );

		return true;
	}

	iRenderCallback* cModel_vulkan::createDefaultsDeferred()
	{
		DF_ProfilingScopeCpu;

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		cPipelineCreateInfo_vulkan pipeline_create_info{ .m_name = "deferred_mesh" };

		pipeline_create_info.m_vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( iMesh::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.m_vertex_input_attribute.emplace_back( 0,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::position ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 1,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::normal ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 2,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tangent ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 3,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::bitangent ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 4,
		                                                            0,
		                                                            vk::Format::eR32G32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iMesh::sVertex, iMesh::sVertex::tex_coords ) ) );

		pipeline_create_info.m_push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
		                                                          0,
		                                                          static_cast< uint32_t >( sizeof( cMesh_vulkan::sPushConstants ) ) );

		cDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eSampler );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 2, vk::DescriptorType::eSampledImage );
		cMesh_vulkan::s_descriptor_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.m_descriptor_layouts.push_back( sFrameData_vulkan::s_vertex_scene_descriptor_set_layout.get() );
		pipeline_create_info.m_descriptor_layouts.push_back( cMesh_vulkan::s_descriptor_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "deferred_mesh.vert" ), helper::util::createShaderModule( "deferred_mesh.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormats( std::vector( 3, graphics_api->getRenderColorFormat() ) );
		pipeline_create_info.setDepthFormat( graphics_api->getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthTest( true, vk::CompareOp::eLessOrEqual );
		pipeline_create_info.disableBlending();

		return cRenderCallbackManager::create( "deferred_mesh", pipeline_create_info, render_callbacks::cDefaultMesh_vulkan::deferredMesh );
	}
}

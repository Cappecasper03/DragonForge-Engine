#include "cPipelineCreateInfo_vulkan.h"

#include <slang-com-ptr.h>
#include <sstream>
#include <vulkan/vulkan.hpp>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	void cPipelineCreateInfo_vulkan::setShaders( const std::string& _vertex_file, const std::string& _fragment_file )
	{
		DF_ProfilingScopeCpu;

		m_shader_stages.clear();
		m_shader_stages.emplace_back( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, createShaderModule( _vertex_file ), "main" );
		m_shader_stages.emplace_back( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, createShaderModule( _fragment_file ), "main" );
	}

	void cPipelineCreateInfo_vulkan::setInputTopology( const vk::PrimitiveTopology _topology, const bool _primitive_restart_enable )
	{
		DF_ProfilingScopeCpu;

		m_input_assembly.topology               = _topology;
		m_input_assembly.primitiveRestartEnable = _primitive_restart_enable;
	}

	void cPipelineCreateInfo_vulkan::setPolygonMode( const vk::PolygonMode _mode, const float _line_width )
	{
		DF_ProfilingScopeCpu;

		m_rasterizer.polygonMode = _mode;
		m_rasterizer.lineWidth   = _line_width;
	}

	void cPipelineCreateInfo_vulkan::setCullMode( const vk::CullModeFlags _cull_mode, const vk::FrontFace _front_face )
	{
		DF_ProfilingScopeCpu;

		m_rasterizer.cullMode  = _cull_mode;
		m_rasterizer.frontFace = _front_face;
	}

	void cPipelineCreateInfo_vulkan::setMultisamplingNone()
	{
		DF_ProfilingScopeCpu;

		m_multisampling.sampleShadingEnable   = false;
		m_multisampling.rasterizationSamples  = vk::SampleCountFlagBits::e1;
		m_multisampling.minSampleShading      = 1;
		m_multisampling.pSampleMask           = nullptr;
		m_multisampling.alphaToCoverageEnable = false;
		m_multisampling.alphaToOneEnable      = false;
	}

	void cPipelineCreateInfo_vulkan::enableBlending()
	{
		DF_ProfilingScopeCpu;

		m_color_blend_attachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		m_color_blend_attachment.blendEnable    = true;
		m_color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
		m_color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
		m_color_blend_attachment.colorBlendOp        = vk::BlendOp::eAdd;
		m_color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eSrcAlpha;
		m_color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
		m_color_blend_attachment.alphaBlendOp        = vk::BlendOp::eAdd;
	}

	void cPipelineCreateInfo_vulkan::disableBlending()
	{
		DF_ProfilingScopeCpu;

		m_color_blend_attachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		m_color_blend_attachment.blendEnable    = false;
	}

	void cPipelineCreateInfo_vulkan::enableDepthTest( const bool _depth_write_enable, const vk::CompareOp _operation )
	{
		DF_ProfilingScopeCpu;

		m_depth_stencil.depthTestEnable       = true;
		m_depth_stencil.depthWriteEnable      = _depth_write_enable;
		m_depth_stencil.depthCompareOp        = _operation;
		m_depth_stencil.depthBoundsTestEnable = false;
		m_depth_stencil.stencilTestEnable     = false;
		m_depth_stencil.front                 = vk::StencilOp::eZero;
		m_depth_stencil.back                  = vk::StencilOp::eZero;
		m_depth_stencil.minDepthBounds        = 0;
		m_depth_stencil.maxDepthBounds        = 1;
	}

	void cPipelineCreateInfo_vulkan::disableDepthTest()
	{
		DF_ProfilingScopeCpu;

		m_depth_stencil.depthTestEnable       = false;
		m_depth_stencil.depthWriteEnable      = false;
		m_depth_stencil.depthCompareOp        = vk::CompareOp::eNever;
		m_depth_stencil.depthBoundsTestEnable = false;
		m_depth_stencil.stencilTestEnable     = false;
		m_depth_stencil.front                 = vk::StencilOp::eZero;
		m_depth_stencil.back                  = vk::StencilOp::eZero;
		m_depth_stencil.minDepthBounds        = 0;
		m_depth_stencil.maxDepthBounds        = 1;
	}

	void cPipelineCreateInfo_vulkan::setColorFormat( const vk::Format _format )
	{
		DF_ProfilingScopeCpu;

		m_color_attachment_formats = { _format };
		m_render_info.setColorAttachmentFormats( m_color_attachment_formats );
	}

	void cPipelineCreateInfo_vulkan::setColorFormats( const std::vector< vk::Format >& _formats )
	{
		DF_ProfilingScopeCpu;

		m_color_attachment_formats = _formats;
		m_render_info.setColorAttachmentFormats( m_color_attachment_formats );
	}

	void cPipelineCreateInfo_vulkan::setDepthFormat( const vk::Format _format )
	{
		DF_ProfilingScopeCpu;

		m_render_info.setDepthAttachmentFormat( _format );
	}

	vk::ShaderModule cPipelineCreateInfo_vulkan::createShaderModule( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::string       slang_shader_path = cFileSystem::getPath( _name + ".slang" );
		std::ifstream     inputFile( slang_shader_path );
		std::stringstream buffer;
		buffer << inputFile.rdbuf();
		std::string originalContent = buffer.str();
		inputFile.close();
		std::string slang_shader_source = "#define DF_Vulkan\n" + originalContent;

		static Slang::ComPtr< slang::IGlobalSession > slang_global_session;
		if( !slang_global_session.get() )
			createGlobalSession( slang_global_session.writeRef() );

		const slang::TargetDesc target_desc{
			.format  = SLANG_SPIRV,
			.profile = slang_global_session->findProfile( "glsl_450" ),
			.flags   = 0,
		};

		const slang::SessionDesc session_desc{
			.targets                 = &target_desc,
			.targetCount             = 1,
			.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
		};

		Slang::ComPtr< slang::ISession > session;
		slang_global_session->createSession( session_desc, session.writeRef() );

		Slang::ComPtr< slang::IBlob > diagnostic_blob;
		slang::IModule* slang_module = session->loadModuleFromSourceString( _name.data(), slang_shader_path.data(), slang_shader_source.data(), diagnostic_blob.writeRef() );

		Slang::ComPtr< slang::IEntryPoint > entry_point;
		slang_module->findEntryPointByName( "main", entry_point.writeRef() );

		std::vector< slang::IComponentType* > component_types;
		component_types.push_back( slang_module );
		component_types.push_back( entry_point );

		Slang::ComPtr< slang::IComponentType > composed_program;
		session->createCompositeComponentType( component_types.data(), static_cast< SlangInt >( component_types.size() ), composed_program.writeRef(), diagnostic_blob.writeRef() );

		Slang::ComPtr< slang::IBlob > spirv_code;
		composed_program->getEntryPointCode( 0, 0, spirv_code.writeRef(), diagnostic_blob.writeRef() );

		const vk::ShaderModuleCreateInfo create_info( vk::ShaderModuleCreateFlags(),
		                                              spirv_code->getBufferSize(),
		                                              static_cast< const uint32_t* >( spirv_code->getBufferPointer() ) );

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		const vk::ShaderModule module = graphics_api->getLogicalDevice().createShaderModule( create_info ).value;
		DF_LogMessage( "Successfully loaded shader and created shader module: {}", _name );
		return module;
	}
}

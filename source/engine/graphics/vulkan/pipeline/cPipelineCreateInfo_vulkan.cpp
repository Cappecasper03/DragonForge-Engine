#include "cPipelineCreateInfo_vulkan.h"

#include <vulkan/vulkan.hpp>

#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	void cPipelineCreateInfo_vulkan::setShaders( const vk::ShaderModule _vertex, const vk::ShaderModule _fragment )
	{
		DF_ProfilingScopeCpu;

		m_shader_stages.clear();
		m_shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eVertex, _vertex ) );
		m_shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eFragment, _fragment ) );
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

	void cPipelineCreateInfo_vulkan::enableBlendingAlphaBlend()
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
}

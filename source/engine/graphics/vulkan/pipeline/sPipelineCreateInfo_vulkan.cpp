#include "sPipelineCreateInfo_vulkan.h"

#include <vulkan/vulkan.hpp>

#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	void sPipelineCreateInfo_vulkan::setShaders( const vk::ShaderModule _vertex, const vk::ShaderModule _fragment )
	{
		DF_ProfilingScopeCpu;

		shader_stages.clear();
		shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eVertex, _vertex ) );
		shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eFragment, _fragment ) );
	}

	void sPipelineCreateInfo_vulkan::setInputTopology( const vk::PrimitiveTopology _topology, const bool _primitive_restart_enable )
	{
		DF_ProfilingScopeCpu;

		input_assembly.topology               = _topology;
		input_assembly.primitiveRestartEnable = _primitive_restart_enable;
	}

	void sPipelineCreateInfo_vulkan::setPolygonMode( const vk::PolygonMode _mode, const float _line_width )
	{
		DF_ProfilingScopeCpu;

		rasterizer.polygonMode = _mode;
		rasterizer.lineWidth   = _line_width;
	}

	void sPipelineCreateInfo_vulkan::setCullMode( const vk::CullModeFlags _cull_mode, const vk::FrontFace _front_face )
	{
		DF_ProfilingScopeCpu;

		rasterizer.cullMode  = _cull_mode;
		rasterizer.frontFace = _front_face;
	}

	void sPipelineCreateInfo_vulkan::setMultisamplingNone()
	{
		DF_ProfilingScopeCpu;

		multisampling.sampleShadingEnable   = false;
		multisampling.rasterizationSamples  = vk::SampleCountFlagBits::e1;
		multisampling.minSampleShading      = 1;
		multisampling.pSampleMask           = nullptr;
		multisampling.alphaToCoverageEnable = false;
		multisampling.alphaToOneEnable      = false;
	}

	void sPipelineCreateInfo_vulkan::enableBlendingAdditive()
	{
		DF_ProfilingScopeCpu;

		color_blend_attachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		color_blend_attachment.blendEnable    = true;
		color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eOne;
		color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eDstAlpha;
		color_blend_attachment.colorBlendOp        = vk::BlendOp::eAdd;
		color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
		color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
		color_blend_attachment.alphaBlendOp        = vk::BlendOp::eAdd;
	}
	void sPipelineCreateInfo_vulkan::enableBlendingAlphaBlend()
	{
		DF_ProfilingScopeCpu;

		color_blend_attachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		color_blend_attachment.blendEnable    = true;
		color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eOneMinusDstAlpha;
		color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eDstAlpha;
		color_blend_attachment.colorBlendOp        = vk::BlendOp::eAdd;
		color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
		color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
		color_blend_attachment.alphaBlendOp        = vk::BlendOp::eAdd;
	}

	void sPipelineCreateInfo_vulkan::disableBlending()
	{
		DF_ProfilingScopeCpu;

		color_blend_attachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		color_blend_attachment.blendEnable    = false;
	}

	void sPipelineCreateInfo_vulkan::enableDepthTest( const bool _depth_write_enable, const vk::CompareOp _operation )
	{
		DF_ProfilingScopeCpu;

		depth_stencil.depthTestEnable       = true;
		depth_stencil.depthWriteEnable      = _depth_write_enable;
		depth_stencil.depthCompareOp        = _operation;
		depth_stencil.depthBoundsTestEnable = false;
		depth_stencil.stencilTestEnable     = false;
		depth_stencil.front                 = vk::StencilOp::eZero;
		depth_stencil.back                  = vk::StencilOp::eZero;
		depth_stencil.minDepthBounds        = 0;
		depth_stencil.maxDepthBounds        = 1;
	}

	void sPipelineCreateInfo_vulkan::disableDepthTest()
	{
		DF_ProfilingScopeCpu;

		depth_stencil.depthTestEnable       = false;
		depth_stencil.depthWriteEnable      = false;
		depth_stencil.depthCompareOp        = vk::CompareOp::eNever;
		depth_stencil.depthBoundsTestEnable = false;
		depth_stencil.stencilTestEnable     = false;
		depth_stencil.front                 = vk::StencilOp::eZero;
		depth_stencil.back                  = vk::StencilOp::eZero;
		depth_stencil.minDepthBounds        = 0;
		depth_stencil.maxDepthBounds        = 1;
	}

	void sPipelineCreateInfo_vulkan::setColorFormat( const vk::Format _format )
	{
		DF_ProfilingScopeCpu;

		color_attachment_formats = { _format };
		render_info.setColorAttachmentFormats( color_attachment_formats );
	}

	void sPipelineCreateInfo_vulkan::setColorFormats( const std::vector< vk::Format >& _formats )
	{
		DF_ProfilingScopeCpu;

		color_attachment_formats = _formats;
		render_info.setColorAttachmentFormats( color_attachment_formats );
	}

	void sPipelineCreateInfo_vulkan::setDepthFormat( const vk::Format _format )
	{
		DF_ProfilingScopeCpu;

		render_info.setDepthAttachmentFormat( _format );
	}
}

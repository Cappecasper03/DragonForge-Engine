#include "sPipelineCreateInfo.h"

#include <tracy/Tracy.hpp>

#include "engine/rendering/vulkan/misc/Helper_vulkan.h"

namespace df::vulkan
{
	void sPipelineCreateInfo::setShaders( const VkShaderModule _vertex, const VkShaderModule _fragment )
	{
		ZoneScoped;

		shader_stages.clear();
		shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( VK_SHADER_STAGE_VERTEX_BIT, _vertex ) );
		shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( VK_SHADER_STAGE_FRAGMENT_BIT, _fragment ) );
	}

	void sPipelineCreateInfo::setInputTopology( const VkPrimitiveTopology _topology, const bool _primitive_restart_enable )
	{
		ZoneScoped;

		input_assembly.topology               = _topology;
		input_assembly.primitiveRestartEnable = _primitive_restart_enable;
	}

	void sPipelineCreateInfo::setpolygonMode( const VkPolygonMode _mode, const float _line_width )
	{
		ZoneScoped;

		rasterizer.polygonMode = _mode;
		rasterizer.lineWidth   = _line_width;
	}

	void sPipelineCreateInfo::setCullMode( const VkCullModeFlags _cull_mode, const VkFrontFace _front_face )
	{
		ZoneScoped;

		rasterizer.cullMode  = _cull_mode;
		rasterizer.frontFace = _front_face;
	}

	void sPipelineCreateInfo::setMultisamplingNone()
	{
		ZoneScoped;

		multisampling.sampleShadingEnable   = false;
		multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading      = 1.0f;
		multisampling.pSampleMask           = nullptr;
		multisampling.alphaToCoverageEnable = false;
		multisampling.alphaToOneEnable      = false;
	}

	void sPipelineCreateInfo::enableBlendingAdditive()
	{
		ZoneScoped;

		color_blend_attachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable         = true;
		color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
		color_blend_attachment.colorBlendOp        = VK_BLEND_OP_ADD;
		color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment.alphaBlendOp        = VK_BLEND_OP_ADD;
	}
	void sPipelineCreateInfo::enableBlendingAlphablend()
	{
		ZoneScoped;

		color_blend_attachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable         = true;
		color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
		color_blend_attachment.colorBlendOp        = VK_BLEND_OP_ADD;
		color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment.alphaBlendOp        = VK_BLEND_OP_ADD;
	}

	void sPipelineCreateInfo::disableBlending()
	{
		ZoneScoped;

		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable    = false;
	}

	void sPipelineCreateInfo::enableDepthtest( const bool _depth_write_enable, const VkCompareOp _operation )
	{
		ZoneScoped;

		depth_stencil.depthTestEnable       = true;
		depth_stencil.depthWriteEnable      = _depth_write_enable;
		depth_stencil.depthCompareOp        = _operation;
		depth_stencil.depthBoundsTestEnable = false;
		depth_stencil.stencilTestEnable     = false;
		depth_stencil.front                 = {};
		depth_stencil.back                  = {};
		depth_stencil.minDepthBounds        = 0;
		depth_stencil.maxDepthBounds        = 1;
	}

	void sPipelineCreateInfo::disableDepthtest()
	{
		ZoneScoped;

		depth_stencil.depthTestEnable       = false;
		depth_stencil.depthWriteEnable      = false;
		depth_stencil.depthCompareOp        = VK_COMPARE_OP_NEVER;
		depth_stencil.depthBoundsTestEnable = false;
		depth_stencil.stencilTestEnable     = false;
		depth_stencil.front                 = {};
		depth_stencil.back                  = {};
		depth_stencil.minDepthBounds        = 0;
		depth_stencil.maxDepthBounds        = 1;
	}

	void sPipelineCreateInfo::setColorFormat( const VkFormat _format )
	{
		ZoneScoped;

		color_attachment_format             = _format;
		render_info.colorAttachmentCount    = 1;
		render_info.pColorAttachmentFormats = &color_attachment_format;
	}

	void sPipelineCreateInfo::setDepthFormat( const VkFormat _format )
	{
		ZoneScoped;

		render_info.depthAttachmentFormat = _format;
	}
}

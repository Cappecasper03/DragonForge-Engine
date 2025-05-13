#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace df::vulkan
{
	class cPipelineCreateInfo_vulkan
	{
	public:
		void setShaders( vk::ShaderModule _vertex, vk::ShaderModule _fragment );
		void setInputTopology( vk::PrimitiveTopology _topology, bool _primitive_restart_enable = false );
		void setPolygonMode( vk::PolygonMode _mode, float _line_width = 1 );
		void setCullMode( vk::CullModeFlags _cull_mode, vk::FrontFace _front_face );
		void setMultisamplingNone();

		void enableBlendingAdditive();
		void enableBlendingAlphaBlend();
		void disableBlending();

		void enableDepthTest( bool _depth_write_enable, vk::CompareOp _operation );
		void disableDepthTest();

		void setColorFormat( vk::Format _format );
		void setColorFormats( const std::vector< vk::Format >& _formats );
		void setDepthFormat( vk::Format _format );

		vk::PipelineRasterizationStateCreateInfo m_rasterizer{};
		vk::PipelineRenderingCreateInfo          m_render_info{};
		vk::PipelineDepthStencilStateCreateInfo  m_depth_stencil{};
		vk::PipelineMultisampleStateCreateInfo   m_multisampling{};
		vk::PipelineInputAssemblyStateCreateInfo m_input_assembly{};
		vk::PipelineColorBlendAttachmentState    m_color_blend_attachment{};
		std::vector< vk::Format >                m_color_attachment_formats{};

		std::vector< vk::PipelineShaderStageCreateInfo >   m_shader_stages{};
		std::vector< vk::PushConstantRange >               m_push_constant_ranges{};
		std::vector< vk::DescriptorSetLayout >             m_descriptor_layouts{};
		std::vector< vk::VertexInputBindingDescription >   m_vertex_input_binding{};
		std::vector< vk::VertexInputAttributeDescription > m_vertex_input_attribute{};

		std::string m_name;
	};
}

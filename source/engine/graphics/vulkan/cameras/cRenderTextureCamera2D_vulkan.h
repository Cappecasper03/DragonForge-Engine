#pragma once

#include "engine/graphics/cameras/cRenderTextureCamera2D.h"
#include "engine/graphics/vulkan/types/sAllocatedBuffer_vulkan.h"

namespace df::vulkan
{
	class cRenderTextureCamera2D_vulkan : public cRenderTextureCamera2D
	{
	public:
		DF_DeleteCopyAndMove( cRenderTextureCamera2D_vulkan );

		cRenderTextureCamera2D_vulkan( const sDescription& _description );
		~cRenderTextureCamera2D_vulkan() override;

		void beginRender( eClearFlags _clear_flags ) override;
		void endRender() override;

		std::vector< vk::DescriptorSet >& getDescriptors() { return m_vertex_scene_descriptor_sets; }

	private:
		sAllocatedBuffer_vulkan          m_vertex_scene_uniform_buffer;
		std::vector< vk::DescriptorSet > m_vertex_scene_descriptor_sets;
	};
}

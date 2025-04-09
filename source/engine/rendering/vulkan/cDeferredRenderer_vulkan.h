#pragma once

#include <glm/ext/matrix_float4x4.hpp>

#include "cRenderer_vulkan.h"
#include "engine/misc/Misc.h"

namespace df::vulkan
{
	class cDeferredRenderer_vulkan final : public cRenderer_vulkan
	{
	public:
		DF_DisableCopyAndMove( cDeferredRenderer_vulkan );

		struct sPushConstants
		{
			glm::mat4 world_matrix;
		};

		explicit cDeferredRenderer_vulkan( const std::string& _window_name );
		~cDeferredRenderer_vulkan() override;

		void beginRendering( int _clear_buffers, const cColor& _color ) override;

		const vk::DescriptorSetLayout& getDeferredLayout() const { return m_deferred_layout.get(); }

	private:
		void renderDeferred( const vk::CommandBuffer& _command_buffer ) override;

		void initializeDeferred() override;

		void createQuadRenderCallback();

		bool                          m_begin_deferred;
		vk::UniqueDescriptorSetLayout m_deferred_layout;
	};
}

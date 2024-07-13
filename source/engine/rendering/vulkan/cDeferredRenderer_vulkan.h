#pragma once

#include <glm/ext/matrix_float4x4.hpp>

#include "cRenderer_vulkan.h"
#include "engine/misc/Misc.h"

namespace df::vulkan
{
	class cDeferredRenderer_vulkan final : public cRenderer_vulkan
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cDeferredRenderer_vulkan )

		struct sDeferredPushConstants
		{
			glm::mat4 world_matrix;
		};

		cDeferredRenderer_vulkan() = default;
		~cDeferredRenderer_vulkan() override;

		const vk::DescriptorSetLayout& getTextureLayout() const { return m_texture_layout.get(); }

	private:
		void renderDeferred() override;

		void initializeDeferred() override;

		void createQuadRenderCallback();

		vk::UniqueDescriptorSetLayout m_texture_layout;
	};
}

#pragma once

#include <string>

#include "engine/core/utils/cColor.h"
#include "engine/graphics/assets/iQuad.h"
#include "sRenderAsset_vulkan.h"

namespace df
{
	struct iRenderCallback;

	class cQuadManager;
}

namespace df::vulkan
{
	class cTexture_vulkan;

	class cQuad_vulkan : public sRenderAsset_vulkan,
						 public iQuad
	{
	public:
		DF_DisableCopyAndMove( cQuad_vulkan );

		cQuad_vulkan( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color = color::white, bool _update_descriptor_set = true );
		~cQuad_vulkan() override = default;

		bool loadTexture( const std::string& _file_path, bool _mipmapped, int _mipmaps, bool _flip_vertically_on_load ) override;

		void render() override;

		static iRenderCallback* createDefaults();
		static void             destroyDefaults();

		const std::vector< vk::DescriptorSet >& getDescriptors() const { return m_descriptors; }
		static vk::DescriptorSetLayout          getLayout() { return s_descriptor_layout.get(); }

	private:
		static iRenderCallback* createDefaultsDeferred();

		std::vector< vk::DescriptorSet >     m_descriptors;
		static vk::UniqueDescriptorSetLayout s_descriptor_layout;
	};
}

#pragma once

#include <string>

#include "core/math/cMatrix.h"
#include "engine/misc/cColor.h"
#include "graphics/assets/iQuad.h"
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

		struct sPushConstants
		{
			cMatrix4f world_matrix;
		};

		cQuad_vulkan( std::string _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color = color::white );
		~cQuad_vulkan() override = default;

		bool loadTexture( const std::string& _file_path, bool _mipmapped, int _mipmaps, bool _flip_vertically_on_load ) override;

		void render() override;

		static iRenderCallback* createDefaults();
		static void             destroyDefaults();

		static vk::DescriptorSetLayout                 getLayout() { return s_descriptor_layout.get(); }
		static const std::vector< vk::DescriptorSet >& getDescriptors() { return s_descriptors; }

	private:
		static iRenderCallback* createDefaultsDeferred();

		static vk::UniqueDescriptorSetLayout    s_descriptor_layout;
		static std::vector< vk::DescriptorSet > s_descriptors;
	};
}

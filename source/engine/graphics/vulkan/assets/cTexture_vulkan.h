﻿#pragma once

#include <string>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/iTexture.h"
#include "engine/graphics/vulkan/types/sAllocatedImage_vulkan.h"

namespace df::vulkan
{
	class cTexture_vulkan : public iTexture
	{
	public:
		DF_DisableCopyAndMove( cTexture_vulkan );

		explicit cTexture_vulkan( std::string _name );
		~cTexture_vulkan() override;

		bool load( const std::string& _file, bool _mipmapped = false, int _mipmaps = 0, bool _flip_vertically_on_load = true ) override;

		void bind( int /*_index*/ = 0 ) override {}
		void unbind( int /*_index*/ = 0 ) override {}

		const sAllocatedImage_vulkan& getImage() const { return m_texture; }

	protected:
		sAllocatedImage_vulkan m_texture;
		std::string            m_path;
	};
}

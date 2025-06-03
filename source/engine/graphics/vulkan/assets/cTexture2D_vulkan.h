#pragma once

#include <string>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/vulkan/types/sAllocatedImage_vulkan.h"

namespace df::vulkan
{
	class cTexture2D_vulkan : public cTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D_vulkan );

		cTexture2D_vulkan( const std::string& _name );
		~cTexture2D_vulkan() override;

		bool loadFromData( const std::string& _file_path, const void* _data, const cVector2i& _size, bool _mipmapped, int _mipmaps ) override;

		void bind( int /*_index*/ = 0 ) const override {}
		void unbind( int /*_index*/ = 0 ) const override {}

		const sAllocatedImage_vulkan& getImage() const { return m_texture; }

	protected:
		sAllocatedImage_vulkan m_texture;
		std::string            m_path;
	};
}

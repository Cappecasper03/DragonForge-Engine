#pragma once

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/vulkan/types/sAllocatedImage_vulkan.h"

namespace df::vulkan
{
	class cTexture2D_vulkan : public cTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D_vulkan );

		cTexture2D_vulkan() = default;
		~cTexture2D_vulkan() override;

		void clear( const cColor& _color ) override;

		void uploadData( const void* _data, sTextureFormat::eFormat _format, unsigned _mip_level, bool _generate_mipmap ) override;

		void bind( int /*_index*/ = 0 ) const override {}
		void unbind( int /*_index*/ = 0 ) const override {}

		const sAllocatedImage_vulkan& getImage() const { return m_texture; }

	protected:
		void initialize( const sDescription& _description ) override;

		sAllocatedImage_vulkan m_texture;
	};
}

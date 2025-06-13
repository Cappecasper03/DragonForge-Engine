#pragma once

#include "engine/graphics/assets/textures/cRenderTexture2D.h"
#include "engine/graphics/vulkan/types/sAllocatedImage_vulkan.h"

namespace df::vulkan
{
	class cRenderTexture2D_vulkan : public cRenderTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cRenderTexture2D_vulkan );

		cRenderTexture2D_vulkan() = default;
		~cRenderTexture2D_vulkan() override;

		void clear( const cColor& _color ) override;

		void bind( int _index = 0 ) const override {}
		void unbind( int _index = 0 ) const override {}

		const sAllocatedImage_vulkan& getImage() const { return m_texture; }

	protected:
		void initialize( const sDescription& _description ) override;

		sAllocatedImage_vulkan m_texture;
	};
}

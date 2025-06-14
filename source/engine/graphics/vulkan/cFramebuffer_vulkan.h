﻿#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/core/math/cVector.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/api/iFramebuffer.h"

namespace df::vulkan
{
	class cTexture2D_vulkan;

	class cFramebuffer_vulkan : public iFramebuffer
	{
	public:
		DF_DeleteCopyAndMove( cFramebuffer_vulkan );

		explicit cFramebuffer_vulkan( uint32_t _num_render_textures = 0, uint32_t _frames_in_flight = 1, const cVector2i& _size = cVector2i( -1, -1 ) );
		~cFramebuffer_vulkan() override;

		const std::vector< cTexture2D_vulkan* >& getCurrentFrameImages( const uint32_t _frame_index ) const { return m_frame_images[ _frame_index ]; }

	private:
		std::vector< std::vector< cTexture2D_vulkan* > > m_frame_images;
	};
}

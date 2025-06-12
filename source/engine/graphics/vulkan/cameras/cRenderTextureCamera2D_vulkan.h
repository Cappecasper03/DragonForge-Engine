#pragma once

#include "engine/graphics/cameras/cRenderTextureCamera2D.h"

namespace df::vulkan
{
	class cRenderTextureCamera2D_vulkan : public cRenderTextureCamera2D
	{
	public:
		DF_DeleteCopyAndMove( cRenderTextureCamera2D_vulkan );

		cRenderTextureCamera2D_vulkan( const sDescription& _description );
		~cRenderTextureCamera2D_vulkan() override = default;

		void bind() override {}
		void unbind() override {}
	};
}

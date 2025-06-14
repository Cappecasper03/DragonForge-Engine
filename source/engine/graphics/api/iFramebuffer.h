#pragma once

#include "engine/core/utils/Misc.h"

namespace df
{
	class cTexture2D;

	class iFramebuffer
	{
	public:
		DF_DeleteCopyAndMove( iFramebuffer );

		iFramebuffer()          = default;
		virtual ~iFramebuffer() = default;

		virtual void bind() {}
		virtual void unbind() {}
	};
}

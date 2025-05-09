﻿#pragma once

#include <string>
#include <vector>

#include "engine/misc/Misc.h"

namespace df
{
	class iTexture;

	class iFramebuffer
	{
	public:
		DF_DisableCopyAndMove( iFramebuffer );

		explicit iFramebuffer( std::string _name );
		virtual ~iFramebuffer();

		virtual void bind() {}
		virtual void unbind() {}

		std::vector< iTexture* > render_textues;
		const std::string        name;
	};
}

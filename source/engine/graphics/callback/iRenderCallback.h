#pragma once

#include <string>

#include "engine/misc/Misc.h"

namespace df
{
	struct iRenderCallback
	{
		DF_DisableCopyAndMove( iRenderCallback );

		explicit iRenderCallback( std::string _name )
			: name( std::move( _name ) )
		{}

		virtual ~iRenderCallback() = default;

		const std::string name;
	};
}

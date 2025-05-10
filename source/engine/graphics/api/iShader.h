#pragma once

#include <string>

#include "engine/core/utils/Misc.h"

namespace df
{
	class iShader
	{
	public:
		DF_DisableCopyAndMove( iShader );

		explicit iShader( std::string _name )
			: name( std::move( _name ) )
		{}

		virtual ~iShader() = default;

		const std::string name;
	};
}

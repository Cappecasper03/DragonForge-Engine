#pragma once

#include <string>

#include "engine/core/utils/Misc.h"

namespace df
{
	class iShader
	{
	public:
		DF_DeleteCopyAndMove( iShader );

		explicit iShader( std::string _name )
			: m_name( std::move( _name ) )
		{}

		virtual ~iShader() = default;

		const std::string m_name;
	};
}

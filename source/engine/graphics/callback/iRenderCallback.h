#pragma once

#include <string>

#include "engine/core/utils/Misc.h"

namespace df
{
	struct iRenderCallback
	{
		DF_DeleteCopyAndMove( iRenderCallback );

		explicit iRenderCallback( std::string _name )
			: m_name( std::move( _name ) )
		{}

		virtual ~iRenderCallback() = default;

		const std::string m_name;
	};
}

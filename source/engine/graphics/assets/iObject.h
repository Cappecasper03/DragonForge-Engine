﻿#pragma once

#include <string>

#include "engine/core/utils/Misc.h"

namespace df
{
	class iObject
	{
	public:
		DF_DeleteCopyAndMove( iObject );

		explicit iObject( const std::string& _name );
		virtual ~iObject() = default;

		virtual void update( float /*_delta_time*/ = 0 ) = 0;

		const std::string m_name;
	};
}

#pragma once

#include <string>

#include "engine/misc/Misc.h"

namespace df
{
	struct iAsset
	{
		DF_DisableCopyAndMove( iAsset );

		explicit iAsset( std::string _name );
		virtual ~iAsset() = default;

		virtual void update( float /*_delta_time*/ = 0 ) {}
		virtual void render() {}

		const std::string name;
	};
}

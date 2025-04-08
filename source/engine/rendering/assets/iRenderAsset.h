#pragma once

#include <string>

#include "iAsset.h"
#include "engine/misc/Misc.h"

namespace df
{
	struct iRenderCallback;

	class cTransform;

	class iRenderAsset : public iAsset
	{
	public:
		DF_DisableCopyAndMove( iRenderAsset );

		explicit iRenderAsset( std::string _name );
		~iRenderAsset() override;

		void update( float /*_delta_time*/ = 0 ) override;
		void render() override = 0;

		cTransform*      transform;
		iRenderCallback* render_callback;
	};
}

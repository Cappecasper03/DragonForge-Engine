#pragma once

#include <string>

#include "engine/misc/Misc.h"
#include "iAsset.h"

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

		cTransform*      transform       = nullptr;
		iRenderCallback* render_callback = nullptr;
	};
}
